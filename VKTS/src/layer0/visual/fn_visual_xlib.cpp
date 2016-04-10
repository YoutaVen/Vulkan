/**
 * VKTS - VulKan ToolS.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) since 2014 Norbert Nopper
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <vkts/vkts.hpp>

#include "../../fn_internal.hpp"

#include "fn_visual_internal.hpp"

#include "../input/GamepadInput.hpp"
#include "../input/KeyInput.hpp"
#include "../input/MouseInput.hpp"
#include "fn_visual_xlib_internal.hpp"

#define VKTS_WINDOWS_MAX_WINDOWS 64

namespace vkts
{

typedef struct _VKTS_NATIVE_DISPLAY_CONTAINER
{

    int32_t index;

    int32_t width;
    int32_t height;

    VkBool32 defaultDisplay;

    RRMode oldMode;

    NativeDisplaySP display;

} VKTS_NATIVE_DISPLAY_CONTAINER;

//

static Display* g_nativeDisplay = nullptr;

static Cursor g_invisibleCursor;

static Atom g_deleteMessage;

//

static int32_t g_numberWindows = 0;

static uint64_t g_windowBits = 0;

//

static NativeDisplaySP g_defaultDisplay;

static std::map<int32_t, VKTS_NATIVE_DISPLAY_CONTAINER*> g_allDisplays;

static SmartPointerMap<Window, NativeWindowSP> g_allWindows;

//

static int32_t visualGetDisplayFrequency(const XRRModeInfo& modeInfo)
{
    if (modeInfo.hTotal != 0 && modeInfo.vTotal != 0)
    {
        return (int32_t)((double) modeInfo.dotClock / ((double) modeInfo.hTotal * (double) modeInfo.vTotal));
    }

    return -1;
}
//

static void _visualEnumerateDisplays()
{
    if (!g_nativeDisplay)
    {
        return;
    }

    XRRScreenResources* screenResources = XRRGetScreenResources(g_nativeDisplay, DefaultRootWindow(g_nativeDisplay));

    if (!screenResources)
    {
        return;
    }

    RROutput primary = XRRGetOutputPrimary(g_nativeDisplay, DefaultRootWindow(g_nativeDisplay));

    int32_t displayIndex = 0;

    //

    for (int32_t i = 0; i < screenResources->ncrtc; i++)
    {
        XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(g_nativeDisplay, screenResources, screenResources->crtcs[i]);

        if (!crtcInfo)
        {
            continue;
        }

        for (int32_t k = 0; k < crtcInfo->noutput; k++)
        {
            XRROutputInfo* outputInfo = XRRGetOutputInfo(g_nativeDisplay, screenResources, crtcInfo->outputs[k]);

            if (!outputInfo)
            {
                continue;
            }

            logPrint(VKTS_LOG_DEBUG, "Visual: Processing display '%s' - %s", outputInfo->name, (outputInfo->connection == RR_Connected ? "Connected" : "Disconnected"));

            if (outputInfo->connection == RR_Connected)
            {
                VKTS_NATIVE_DISPLAY_CONTAINER* currentDisplayContainer = new VKTS_NATIVE_DISPLAY_CONTAINER();

                if (currentDisplayContainer)
                {
                    currentDisplayContainer->index = displayIndex;

                    currentDisplayContainer->width = crtcInfo->width;
                    currentDisplayContainer->height = crtcInfo->height;

                    currentDisplayContainer->defaultDisplay = (primary == crtcInfo->outputs[k]);

                    currentDisplayContainer->oldMode = crtcInfo->mode;

                    currentDisplayContainer->display = nullptr;

                    g_allDisplays[displayIndex] = currentDisplayContainer;
                }

                displayIndex++;
            }

            XRRFreeOutputInfo(outputInfo);
        }

        XRRFreeCrtcInfo(crtcInfo);
    }

    XRRFreeScreenResources(screenResources);
}

static void _visualRevertFullscreen(const INativeDisplayWP& display)
{
    const auto sharedDisplay = display.lock();

    if (!sharedDisplay.get())
    {
        return;
    }

    const auto currentDisplayWalker = g_allDisplays.find(sharedDisplay->getIndex());

    if (currentDisplayWalker != g_allDisplays.end()
            && currentDisplayWalker->second)
    {
        XRRScreenResources* screenResources = XRRGetScreenResources(g_nativeDisplay, DefaultRootWindow(g_nativeDisplay));

        if (screenResources)
        {
            int32_t displayIndex = 0;

            bool found = false;

            //

            for (int32_t i = 0; i < screenResources->ncrtc; i++)
            {
                XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(g_nativeDisplay, screenResources, screenResources->crtcs[i]);

                if (!crtcInfo)
                {
                    continue;
                }

                for (int32_t k = 0; k < crtcInfo->noutput; k++)
                {
                    XRROutputInfo* outputInfo = XRRGetOutputInfo(g_nativeDisplay, screenResources, crtcInfo->outputs[k]);

                    if (!outputInfo)
                    {
                        continue;
                    }

                    if (outputInfo->connection == RR_Connected)
                    {
                        if (displayIndex == sharedDisplay->getIndex())
                        {
                            found = true;

                            XRRSetCrtcConfig(g_nativeDisplay, screenResources, screenResources->crtcs[i], CurrentTime, crtcInfo->x, crtcInfo->y, currentDisplayWalker->second->oldMode, crtcInfo->rotation, crtcInfo->outputs, crtcInfo->noutput);

                            break;
                        }

                        displayIndex++;
                    }

                    XRRFreeOutputInfo(outputInfo);

                    if (found)
                    {
                        break;
                    }
                }

                XRRFreeCrtcInfo(crtcInfo);

                if (found)
                {
                    break;
                }
            }

            XRRFreeScreenResources(screenResources);
        }
    }
}

static void _visualProcessMessages(const XEvent* msg)
{
    if (!msg)
    {
        return;
    }

    Window currentNativeWindow = 0;

    switch (msg->type)
    {
        case ConfigureNotify:
            currentNativeWindow = msg->xconfigure.window;
            break;
        case MotionNotify:
            currentNativeWindow = msg->xmotion.window;
            break;
        case ButtonPress:
        case ButtonRelease:
            currentNativeWindow = msg->xbutton.window;
            break;
        case KeyPress:
        case KeyRelease:
            currentNativeWindow = msg->xkey.window;
            break;
    }

    NativeWindowSP currentWindow;

    if (currentNativeWindow)
    {
        auto windowIndex = g_allWindows.find(currentNativeWindow);

        if (windowIndex != g_allWindows.size())
        {
            if (g_allWindows.valueAt(windowIndex).get())
            {
                currentWindow = g_allWindows.valueAt(windowIndex);
            }
        }
    }

    switch (msg->type)
    {
        // No close by purpose.

        // No exposure by purpose.

        case ConfigureNotify:
        {
            if (currentWindow.get())
            {
                uint32_t width = (uint32_t)msg->xconfigure.width;
                uint32_t height = (uint32_t)msg->xconfigure.height;

                if (!currentWindow->isFullscreen())
                {
                    currentWindow->setDimension(glm::uvec2(width, height));
                }
                else if (currentWindow->getTempDimension().x != width || currentWindow->getTempDimension().y != height)
                {
                    currentWindow->setTempDimension(glm::uvec2(width, height));

                    XRaiseWindow(g_nativeDisplay, currentNativeWindow);
                }
            }

            return;
        }
        case MotionNotify:
        {
            if (currentWindow.get())
            {
                int32_t x = msg->xmotion.x;
                int32_t y = msg->xmotion.y;

                currentWindow->getMouseInput().setLocation(glm::ivec2(x, y));
            }

            return;
        }
        case ButtonPress:
        {
            if (currentWindow.get())
            {
                switch (msg->xbutton.button)
                {
                    case 1:
                        currentWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_LEFT, VK_TRUE);
                        break;
                    case 2:
                        currentWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_MIDDLE, VK_TRUE);
                        break;
                    case 3:
                        currentWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_RIGHT, VK_TRUE);
                        break;
                }
            }

            return;
        }
        case ButtonRelease:
        {
            if (currentWindow.get())
            {
                switch (msg->xbutton.button)
                {
                    case 1:
                        currentWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_LEFT, VK_FALSE);
                        break;
                    case 2:
                        currentWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_MIDDLE, VK_FALSE);
                        break;
                    case 3:
                        currentWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_RIGHT, VK_FALSE);
                        break;
                    case 4:
                        currentWindow->getMouseInput().setMouseWheel(1);
                        break;
                    case 5:
                        currentWindow->getMouseInput().setMouseWheel(-1);
                        break;
                }
            }

            return;
        }
        case KeyPress:
        {
            int32_t key = _visualTranslateKey(msg->xkey.keycode);

            if (key == VKTS_KEY_ESCAPE)
            {
                XEvent msg;

                memset(&msg, 0, sizeof(msg));
                msg.xclient.type = ClientMessage;
                msg.xclient.display = g_nativeDisplay;
                msg.xclient.window = currentNativeWindow;
                msg.xclient.message_type = XInternAtom(g_nativeDisplay, "_NET_CLOSE_WINDOW", False);
                msg.xclient.format = 32;
                msg.xclient.data.l[0] = CurrentTime;
                msg.xclient.data.l[1] = DefaultRootWindow(g_nativeDisplay);
                XSendEvent(g_nativeDisplay, DefaultRootWindow(g_nativeDisplay), False, SubstructureRedirectMask | SubstructureNotifyMask, &msg);

                XSync(g_nativeDisplay, False);

                return;
            }

            if (currentWindow.get() && key != VKTS_KEY_UNKNOWN)
            {

                currentWindow->getKeyInput().setKey(key, VK_TRUE);
            }

            break;
        }
        case KeyRelease:
        {
            int32_t key = _visualTranslateKey(msg->xkey.keycode);

            if (currentWindow.get() && key != VKTS_KEY_UNKNOWN)
            {
                currentWindow->getKeyInput().setKey(key, VK_FALSE);
            }

            break;
        }
    }
}

//

VkBool32 VKTS_APIENTRY _visualInit(const VkInstance instance, const VkPhysicalDevice physicalDevice)
{
	if (!_visualInitGamepad(instance, physicalDevice))
	{
		return VK_FALSE;
	}

    //

    g_nativeDisplay = XOpenDisplay(0);

    if (!g_nativeDisplay)
    {
        return VK_FALSE;
    }

    //

    const char data[1] = { 0 };
    Pixmap pixmap;

    pixmap = XCreateBitmapFromData(g_nativeDisplay, DefaultRootWindow(g_nativeDisplay), data, 1, 1);

    if (!pixmap)
    {
        return VK_FALSE;
    }

    XColor color;
    memset(&color, 0, sizeof(XColor));

    g_invisibleCursor = XCreatePixmapCursor(g_nativeDisplay, pixmap, pixmap, &color, &color, 0, 0);

    XFreePixmap(g_nativeDisplay, pixmap);

    //

    g_deleteMessage = XInternAtom(g_nativeDisplay, "WM_DELETE_WINDOW", False);

    _visualEnumerateDisplays();

    return _visualInitKey(g_nativeDisplay);
}

VkBool32 VKTS_APIENTRY _visualDispatchMessages()
{
    XEvent msg;

    while (XPending(g_nativeDisplay) > 0)
    {
        XNextEvent(g_nativeDisplay, &msg);

        if (msg.type == ClientMessage && msg.xclient.data.l[0] == (long) g_deleteMessage)
        {
            return VK_FALSE;
        }
        else
        {
            _visualProcessMessages(&msg);
        }
    }

    //

    _visualDispatchMessagesGamepad();

    //

    for (size_t i = 0; i < g_allWindows.values().size(); i++)
    {
        if (g_allWindows.valueAt(i)->isInvisibleCursor())
        {
            XWarpPointer(g_nativeDisplay, None, g_allWindows.valueAt(i)->getNativeWindow(), 0, 0, 0, 0, g_allWindows.valueAt(i)->getDimension().x / 2, g_allWindows.valueAt(i)->getDimension().y / 2);

            break;
        }
    }

    return VK_TRUE;
}

//

int32_t VKTS_APIENTRY _visualGetNumberDisplays()
{
    return g_allDisplays.size();
}

INativeDisplayWP VKTS_APIENTRY _visualCreateDefaultDisplay()
{
    if (g_defaultDisplay.get())
    {
        return g_defaultDisplay;
    }

    VKTS_NATIVE_DISPLAY_CONTAINER* currentDisplayContainer = nullptr;

    for (const auto& walkerDisplayContainer : g_allDisplays)
    {
        if (walkerDisplayContainer.second)
        {
            if (walkerDisplayContainer.second->defaultDisplay)
            {
                if (walkerDisplayContainer.second->display.get())
                {
                    return INativeDisplayWP();
                }

                currentDisplayContainer = walkerDisplayContainer.second;
            }
        }
    }

    if (!currentDisplayContainer)
    {
        return INativeDisplayWP();
    }

    int32_t displayIndex = currentDisplayContainer->index;
    int32_t width = currentDisplayContainer->width;
    int32_t height = currentDisplayContainer->height;

    auto currentDisplay = NativeDisplaySP(new NativeDisplay(displayIndex, VK_TRUE, width, height, g_nativeDisplay));

    if (!currentDisplay.get())
    {
        return INativeDisplayWP();
    }

    currentDisplayContainer->display = currentDisplay;

    //

    g_defaultDisplay = currentDisplay;

    return currentDisplay;
}

INativeDisplayWP VKTS_APIENTRY _visualCreateDisplay(const int32_t displayIndex)
{
    if (displayIndex < 0 || displayIndex >= _visualGetNumberDisplays())
    {
        return INativeDisplayWP();
    }

    //

    VKTS_NATIVE_DISPLAY_CONTAINER* currentDisplayContainer = nullptr;

    auto walkerDisplayContainer = g_allDisplays.find(displayIndex);

    if (walkerDisplayContainer == g_allDisplays.end())
    {
        return INativeDisplayWP();
    }

    currentDisplayContainer = walkerDisplayContainer->second;

    if (!currentDisplayContainer)
    {
        return INativeDisplayWP();
    }

    if (currentDisplayContainer->display.get())
    {
        return currentDisplayContainer->display;
    }

    //

    int32_t width = currentDisplayContainer->width;
    int32_t height = currentDisplayContainer->height;

    auto currentDisplay = NativeDisplaySP(new NativeDisplay(displayIndex, VK_TRUE, width, height, g_nativeDisplay));

    if (!currentDisplay.get())
    {
        return INativeDisplayWP();
    }

    currentDisplayContainer->display = currentDisplay;

    //

    if (currentDisplayContainer->defaultDisplay)
    {
        g_defaultDisplay = currentDisplay;
    }

    return currentDisplay;
}

NativeDisplaySP VKTS_APIENTRY _visualGetDisplay(const int32_t displayIndex)
{
    auto foundDisplayContainer = g_allDisplays.find(displayIndex);

    if (foundDisplayContainer != g_allDisplays.end())
    {
        return foundDisplayContainer->second->display;
    }

    return NativeDisplaySP();
}

const SmartPointerVector<NativeDisplaySP>& VKTS_APIENTRY _visualGetActiveDisplays()
{
    static SmartPointerVector<NativeDisplaySP> displayList;

    displayList.clear();

    for (const auto currentPair : g_allDisplays)
    {
        displayList.append(currentPair.second->display);
    }

    return displayList;
}

void VKTS_APIENTRY _visualDestroyDisplay(const NativeDisplaySP& display)
{
    if (!display.get())
    {
        return;
    }

    if (display.get() == g_defaultDisplay.get())
    {
        g_defaultDisplay = NativeDisplaySP();
    }

    auto walkerDisplayContainer = g_allDisplays.find(display->getIndex());

    if (walkerDisplayContainer == g_allDisplays.end())
    {
        return;
    }

    if (walkerDisplayContainer->second)
    {
        walkerDisplayContainer->second->display = NativeDisplaySP();
    }
}

//

INativeWindowWP VKTS_APIENTRY _visualCreateWindow(const INativeDisplayWP& display, const char* title, const int32_t width, const int32_t height, const VkBool32 fullscreen, const VkBool32 resize, const VkBool32 invisibleCursor)
{
    const auto sharedDisplay = display.lock();

    if (!sharedDisplay.get())
    {
        logPrint(VKTS_LOG_ERROR, "Visual: No display.");

        return INativeWindowWP();
    }

    if (sharedDisplay->getIndex() < 0 || sharedDisplay->getIndex() >= _visualGetNumberDisplays())
    {
        logPrint(VKTS_LOG_ERROR, "Visual: Invalid display index.");

        return INativeWindowWP();
    }

    auto walkerDisplayContainer = g_allDisplays.find(sharedDisplay->getIndex());

    if (walkerDisplayContainer == g_allDisplays.end() || !walkerDisplayContainer->second || !walkerDisplayContainer->second->display)
    {
        logPrint(VKTS_LOG_ERROR, "Visual: Display not known.");

        return INativeWindowWP();
    }

    //

    VkBool32 finalResize = resize && !fullscreen;

    //

    uint32_t currentWindowBit = 1;
    int32_t currentWindowIndex = 0;

    if (g_numberWindows == VKTS_WINDOWS_MAX_WINDOWS)
    {
        logPrint(VKTS_LOG_ERROR, "Visual: Maximum windows reached.");

        return INativeWindowWP();
    }

    while (currentWindowBit & g_windowBits)
    {
        currentWindowBit = currentWindowBit << 1;

        currentWindowIndex++;
    }

    //

    if (fullscreen)
    {
        XRRScreenResources* screenResources = XRRGetScreenResources(g_nativeDisplay, DefaultRootWindow(g_nativeDisplay));

        if (!screenResources)
        {
            logPrint(VKTS_LOG_ERROR, "Visual: Could not get screen resources.");

            return INativeWindowWP();
        }

        bool found = false;

        int32_t displayIndex = 0;

        //

        for (int32_t i = 0; i < screenResources->ncrtc; i++)
        {
            XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(g_nativeDisplay, screenResources, screenResources->crtcs[i]);

            if (!crtcInfo)
            {
                continue;
            }

            for (int32_t k = 0; k < crtcInfo->noutput; k++)
            {
                XRROutputInfo* outputInfo = XRRGetOutputInfo(g_nativeDisplay, screenResources, crtcInfo->outputs[k]);

                if (!outputInfo)
                {
                    continue;
                }

                if (outputInfo->connection == RR_Connected)
                {
                    if (displayIndex == sharedDisplay->getIndex())
                    {
                        int32_t displayFrequency = 0;

                        RRMode mode;

                        for (int32_t m = 0; m < screenResources->nmode; m++)
                        {
                            if (screenResources->modes[m].width != (uint32_t) width || screenResources->modes[m].height != (uint32_t) height)
                            {
                                continue;
                            }

                            for (int32_t n = 0; n < outputInfo->nmode; n++)
                            {
                                if (outputInfo->modes[n] == screenResources->modes[m].id)
                                {
                                    if (!found)
                                    {
                                        found = true;

                                        mode = outputInfo->modes[n];
                                    }

                                    int32_t currentDisplayFrequency = visualGetDisplayFrequency(screenResources->modes[m]);

                                    if (currentDisplayFrequency > displayFrequency)
                                    {
                                        displayFrequency = currentDisplayFrequency;

                                        mode = outputInfo->modes[n];
                                    }
                                }
                            }
                        }

                        if (found)
                        {
                            if (XRRSetCrtcConfig(g_nativeDisplay, screenResources, screenResources->crtcs[i], CurrentTime, crtcInfo->x, crtcInfo->y, mode, crtcInfo->rotation, crtcInfo->outputs, crtcInfo->noutput) != RRSetConfigSuccess)
                            {
                                found = false;
                            }
                        }
                    }

                    displayIndex++;
                }

                XRRFreeOutputInfo(outputInfo);

                if (found)
                {
                    break;
                }
            }

            XRRFreeCrtcInfo(crtcInfo);

            if (found)
            {
                break;
            }
        }

        XRRFreeScreenResources(screenResources);

        if (!found)
        {
            logPrint(VKTS_LOG_ERROR, "Visual: Could not find full screen mode.");

            return INativeWindowWP();
        }
    }

    //

    long defaultScreen;
    XVisualInfo visualInfoTemplate;
    int numberOfVisuals;
    XVisualInfo* visualInfo;
    Colormap colormap;
    XSetWindowAttributes windowAttributes;

    defaultScreen = DefaultScreen(g_nativeDisplay);

    long visualMask = VisualScreenMask;
    visualInfoTemplate.screen = defaultScreen;

    visualInfo = XGetVisualInfo(g_nativeDisplay, visualMask, &visualInfoTemplate, &numberOfVisuals);

    if (!visualInfo)
    {
        logPrint(VKTS_LOG_ERROR, "Could not get visual info");

        return INativeWindowWP();
    }

    colormap = XCreateColormap(g_nativeDisplay, RootWindow(g_nativeDisplay, defaultScreen), visualInfo->visual, AllocNone);

    windowAttributes.colormap = colormap;
    windowAttributes.background_pixel = 0xFFFFFFFF;
    windowAttributes.border_pixel = 0;
    windowAttributes.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask | PointerMotionMask | StructureNotifyMask | ExposureMask;

    auto currentContainerWalker = g_allDisplays.find(sharedDisplay->getIndex());

    if (currentContainerWalker == g_allDisplays.end() || !currentContainerWalker->second)
    {
        logPrint(VKTS_LOG_ERROR, "Could not find native display");

        return INativeWindowWP();
    }

    int32_t xoffset = 0;
    int32_t yoffset = 0;

    //

    // Move to correct display.

    XRRScreenResources* screenResources = XRRGetScreenResources(g_nativeDisplay, DefaultRootWindow(g_nativeDisplay));

    if (screenResources)
    {
        int32_t displayIndex = 0;

        //

        for (int32_t i = 0; i < screenResources->ncrtc; i++)
        {
            XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(g_nativeDisplay, screenResources, screenResources->crtcs[i]);

            if (!crtcInfo)
            {
                continue;
            }

            for (int32_t k = 0; k < crtcInfo->noutput; k++)
            {
                XRROutputInfo* outputInfo = XRRGetOutputInfo(g_nativeDisplay, screenResources, crtcInfo->outputs[k]);

                if (!outputInfo)
                {
                    continue;
                }

                if (outputInfo->connection == RR_Connected)
                {
                    if (displayIndex == sharedDisplay->getIndex())
                    {
                        xoffset = crtcInfo->x;
                        yoffset = crtcInfo->y;
                    }

                    displayIndex++;
                }

                XRRFreeOutputInfo(outputInfo);
            }

            XRRFreeCrtcInfo(crtcInfo);
        }

        XRRFreeScreenResources(screenResources);
    }

    //

    Window nativeWindow = XCreateWindow(g_nativeDisplay, RootWindow(g_nativeDisplay, defaultScreen), 0, 0, width, height, 0, visualInfo->depth, InputOutput, visualInfo->visual, CWBackPixel | CWBorderPixel | CWEventMask | CWColormap, &windowAttributes);

    XSetStandardProperties(g_nativeDisplay, nativeWindow, title, "", 0, 0, 0, 0);
    XMapWindow(g_nativeDisplay, nativeWindow);
    XSetWMColormapWindows(g_nativeDisplay, nativeWindow, &nativeWindow, 1);
    XMoveWindow(g_nativeDisplay, nativeWindow, xoffset, yoffset);

    if (!finalResize && !fullscreen)
    {
        XSizeHints* sizeHints = XAllocSizeHints();
        sizeHints->flags |= (PMinSize | PMaxSize);
        sizeHints->min_width = sizeHints->max_width = width;
        sizeHints->min_height = sizeHints->max_height = height;
        XSetWMNormalHints(g_nativeDisplay, nativeWindow, sizeHints);
        XFree(sizeHints);
    }

    XFlush(g_nativeDisplay);

    XSetWMProtocols(g_nativeDisplay, nativeWindow, &g_deleteMessage, 1);

    if (invisibleCursor)
    {
        XDefineCursor(g_nativeDisplay, nativeWindow, g_invisibleCursor);
    }

    //

    auto window = NativeWindowSP(new NativeWindow(display, nativeWindow, currentWindowIndex, title, width, height, fullscreen, finalResize, invisibleCursor));

    if (!window.get())
    {
        logPrint(VKTS_LOG_ERROR, "Visual: Could not create native window.");

        if (invisibleCursor)
        {
            XUndefineCursor(g_nativeDisplay, nativeWindow);
        }

        XDestroyWindow(g_nativeDisplay, nativeWindow);

        if (fullscreen)
        {
            _visualRevertFullscreen(display);
        }

        return INativeWindowWP();
    }

    if (g_allWindows.find(nativeWindow) != g_allWindows.size())
    {
        logPrint(VKTS_LOG_ERROR, "Visual: Native window already in list.");

        _visualDestroyWindow(window);

        return INativeWindowWP();
    }

    g_allWindows[nativeWindow] = window;

    //

    g_windowBits |= currentWindowBit;

    //

    if (fullscreen)
    {
        Atom stateAtom = XInternAtom(g_nativeDisplay, "_NET_WM_STATE", False);
        Atom stateFullscreenAtom = XInternAtom(g_nativeDisplay, "_NET_WM_STATE_FULLSCREEN", False);

        XEvent xev;
        memset(&xev, 0, sizeof(xev));
        xev.type = ClientMessage;
        xev.xclient.window = nativeWindow;
        xev.xclient.message_type = stateAtom;
        xev.xclient.format = 32;
        xev.xclient.data.l[0] = 1;
        xev.xclient.data.l[1] = stateFullscreenAtom;
        xev.xclient.data.l[2] = 0;

        XSendEvent(g_nativeDisplay, DefaultRootWindow(g_nativeDisplay), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);

        XFlush(g_nativeDisplay);

        //

        walkerDisplayContainer->second->display->setDimension(glm::ivec2(width, height));
    }

    // If resolution changed, adjust all - not just for this - window positions.

    if (fullscreen)
    {
        XRRScreenResources* screenResources = XRRGetScreenResources(g_nativeDisplay, DefaultRootWindow(g_nativeDisplay));

        if (screenResources)
        {
            int32_t displayIndex = 0;

            for (int32_t i = 0; i < screenResources->noutput; i++)
            {
                XRROutputInfo* outputInfo = XRRGetOutputInfo(g_nativeDisplay, screenResources, screenResources->outputs[i]);

                if (!outputInfo)
                {
                    continue;
                }

                if (outputInfo->connection == RR_Connected)
                {
                    for (int32_t k = 0; k < outputInfo->ncrtc; k++)
                    {
                        XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(g_nativeDisplay, screenResources, screenResources->crtcs[k]);

                        if (!crtcInfo)
                        {
                            continue;
                        }

                        if (crtcInfo->noutput <= 0)
                        {
                            XRRFreeCrtcInfo(crtcInfo);

                            continue;
                        }

                        int32_t xoffset = crtcInfo->x;
                        int32_t yoffset = crtcInfo->y;

                        for (size_t index = 0; index < g_allWindows.size(); index++)
                        {
                            if (g_allWindows.valueAt(index).get())
                            {
                                const auto sharedDisplayWindow = g_allWindows.valueAt(index).get()->getNativeDisplay().lock();

                                if (sharedDisplayWindow.get())
                                {
                                    if (displayIndex == sharedDisplayWindow->getIndex())
                                    {
                                        XMoveWindow(g_nativeDisplay, g_allWindows.keyAt(index), xoffset, yoffset);
                                    }
                                }
                            }
                        }

                        displayIndex++;

                        XRRFreeCrtcInfo(crtcInfo);
                    }
                }

                XRRFreeOutputInfo(outputInfo);
            }

            XRRFreeScreenResources(screenResources);

            //

            XFlush(g_nativeDisplay);
        }
    }

    return window;
}

NativeWindowSP VKTS_APIENTRY _visualGetWindow(const int32_t windowIndex)
{
    for (size_t index = 0; index < g_allWindows.size(); index++)
    {
        if (g_allWindows.valueAt(index).get())
        {
            if (g_allWindows.valueAt(index)->getIndex() == windowIndex)
            {
                return g_allWindows.valueAt(index);
            }
        }
    }

    return NativeWindowSP();
}

const SmartPointerVector<NativeWindowSP>& VKTS_APIENTRY _visualGetActiveWindows()
{
    return g_allWindows.values();
}

void VKTS_APIENTRY _visualDestroyWindow(const NativeWindowSP& window)
{
    if (!window.get())
    {
        return;
    }

    if (window->getNativeWindow())
    {
        Window nativeWindow = window->getNativeWindow();

        if (window->isInvisibleCursor())
        {
            XUndefineCursor(g_nativeDisplay, nativeWindow);
        }

        XDestroyWindow(g_nativeDisplay, nativeWindow);
    }

    uint32_t currentNegativeWindowBit = ~(1 << window->getIndex());

    g_windowBits &= currentNegativeWindowBit;

    //

    if (window->isFullscreen())
    {
        _visualRevertFullscreen(window->getNativeDisplay());
    }

    if (window->getNativeWindow())
    {
        g_allWindows[window->getNativeWindow()].reset();
        g_allWindows.remove(window->getNativeWindow());
    }
}

//

void VKTS_APIENTRY _visualTerminate()
{
    while (g_allWindows.size() > 0)
    {
        _visualDestroyWindow(g_allWindows.valueAt(g_allWindows.size() - 1));
    }
    g_allWindows.clear();

    //

    for (auto& walkerDisplayContainer : g_allDisplays)
    {
        if (walkerDisplayContainer.second)
        {
            _visualDestroyDisplay(walkerDisplayContainer.second->display);

            delete walkerDisplayContainer.second;
        }
    }
    g_allDisplays.clear();

    g_defaultDisplay.reset();

    //

    if (g_nativeDisplay)
    {
        XFreeCursor(g_nativeDisplay, g_invisibleCursor);

        XCloseDisplay(g_nativeDisplay);

        g_nativeDisplay = nullptr;
    }

    //

    _visualTerminateGamepad();
}

//

void VKTS_APIENTRY _visualGamepadSetButton(const int32_t gamepadIndex, const int32_t buttonIndex, const VkBool32 pressed)
{
    for (size_t index = 0; index < g_allWindows.size(); index++)
    {
        g_allWindows.valueAt(index)->getGamepadInput(gamepadIndex).setButton(buttonIndex, pressed);
    }
}

void VKTS_APIENTRY _visualGamepadSetAxis(const int32_t gamepadIndex, const int32_t axisIndex, const float value)
{
    for (size_t index = 0; index < g_allWindows.size(); index++)
    {
        g_allWindows.valueAt(index)->getGamepadInput(gamepadIndex).setAxis(axisIndex, value);
    }
}

}
