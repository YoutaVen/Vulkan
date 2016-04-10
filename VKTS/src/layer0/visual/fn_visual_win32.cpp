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

#include "fn_visual_internal.hpp"

#include "fn_visual_win32_internal.hpp"

#include "../input/GamepadInput.hpp"
#include "../input/KeyInput.hpp"
#include "../input/MouseInput.hpp"

#define VKTS_WINDOWS_MAX_WINDOWS 64
#define VKTS_WINDOWS_MAX_CHARS 2

namespace vkts
{

static const char* VKTS_WINDOWS_CLASSNAME = "VKTS";

//

typedef struct _VKTS_NATIVE_DISPLAY_CONTAINER
{

    CHAR deviceName[32];

    int32_t width;
    int32_t height;

    NativeDisplaySP display;

} VKTS_NATIVE_DISPLAY_CONTAINER;

//

static int32_t g_numberWindows = 0;

static uint64_t g_windowBits = 0;

//

static NativeDisplaySP g_defaultDisplay = nullptr;

static std::map<int32_t, VKTS_NATIVE_DISPLAY_CONTAINER*> g_allDisplays;

static SmartPointerMap<HWND, NativeWindowSP> g_allWindows;

//

static VkBool32 g_gamepadAvailable[VKTS_MAX_GAMEPADS];

//

static LRESULT CALLBACK _visualProcessMessages(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
    NativeWindowSP currentWindow;

    auto windowIndex = g_allWindows.find(hWnd);

    if (windowIndex != g_allWindows.size())
    {
        if (g_allWindows.valueAt(windowIndex).get())
        {
            currentWindow = g_allWindows.valueAt(windowIndex);
        }
    }

    switch (uiMsg)
    {
        case WM_CLOSE:
        {
            PostQuitMessage(0);

            return 0;
        }
        case WM_SIZE:
        {
            if (currentWindow.get())
            {
                uint32_t width = LOWORD(lParam);
                uint32_t height = HIWORD(lParam);

                if (!currentWindow->isFullscreen())
                {
                    currentWindow->setDimension(glm::uvec2(width, height));
                }
                else if (currentWindow->getTempDimension().x != width || currentWindow->getTempDimension().y != height)
                {
                    currentWindow->setTempDimension(glm::uvec2(width, height));

                    SetForegroundWindow(hWnd);
                }
            }

            return 0;
        }
        case WM_MOUSEMOVE:
        {
            if (currentWindow.get())
            {
                int32_t x = LOWORD(lParam);
                int32_t y = HIWORD(lParam);

                currentWindow->getMouseInput().setLocation(glm::ivec2(x, y));
            }

            return 0;
        }
        case WM_MOUSEWHEEL:
        {
            if (currentWindow.get())
            {
                int32_t mouseWheel = ((SHORT)HIWORD(wParam)) / WHEEL_DELTA;

                currentWindow->getMouseInput().setMouseWheel(mouseWheel);
            }

            return 0;
        }
        case WM_LBUTTONDOWN:
        {
            SetCapture(hWnd);

            if (currentWindow.get())
            {
                currentWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_LEFT, VK_TRUE);
            }

            return 0;
        }
        case WM_LBUTTONUP:
        {
            ReleaseCapture();

            if (currentWindow.get())
            {
                currentWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_LEFT, VK_FALSE);
            }

            return 0;
        }
        case WM_MBUTTONDOWN:
        {
            SetCapture(hWnd);

            if (currentWindow.get())
            {
                currentWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_MIDDLE, VK_TRUE);
            }

            return 0;
        }
        case WM_MBUTTONUP:
        {
            ReleaseCapture();

            if (currentWindow.get())
            {
                currentWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_MIDDLE, VK_FALSE);
            }

            return 0;
        }
        return 0;
        case WM_RBUTTONDOWN:
        {
            SetCapture(hWnd);

            if (currentWindow.get())
            {
                currentWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_RIGHT, VK_TRUE);
            }

            return 0;
        }
        case WM_RBUTTONUP:
        {
            ReleaseCapture();

            if (currentWindow.get())
            {
                currentWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_RIGHT, VK_FALSE);
            }

            return 0;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            int32_t key = _visualTranslateKey(wParam, lParam);

            if (key == VKTS_KEY_ESCAPE)
            {
                PostQuitMessage(0);

                return 0;
            }

            if (currentWindow.get() && key != VKTS_KEY_UNKNOWN)
            {
                currentWindow->getKeyInput().setKey(key, VK_TRUE);
            }

            break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            int32_t key = _visualTranslateKey(wParam, lParam);

            if (currentWindow.get() && key != VKTS_KEY_UNKNOWN)
            {
                currentWindow->getKeyInput().setKey(key, VK_FALSE);
            }

            break;
        }
    }

    return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

//

VkBool32 VKTS_APIENTRY _visualInit(const VkInstance instance, const VkPhysicalDevice physicalDevice)
{
    XINPUT_STATE xinputState;

    for (int32_t i = 0; i < VKTS_MAX_GAMEPADS; i++)
    {
        g_gamepadAvailable[i] = VK_FALSE;

        ZeroMemory(&xinputState, sizeof(XINPUT_STATE));
        g_gamepadAvailable[i] = XInputGetState(i, &xinputState) == ERROR_SUCCESS;
    }

    //

    return _visualInitKey();
}

VkBool32 VKTS_APIENTRY _visualDispatchMessages()
{
    MSG msg;

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return VK_FALSE;
        }
        else
        {
            DispatchMessage(&msg);
        }
    }

    //

    float axisValue;

    XINPUT_STATE xinputState;

    for (int32_t i = 0; i < VKTS_MAX_GAMEPADS; i++)
    {
        if (g_gamepadAvailable[i])
        {
            ZeroMemory(&xinputState, sizeof(XINPUT_STATE));
            XInputGetState(i, &xinputState);

            for (size_t index = 0; index < g_allWindows.size(); index++)
            {
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_A, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_A ? VK_TRUE : VK_FALSE);
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_B, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_B ? VK_TRUE : VK_FALSE);
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_X, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_X ? VK_TRUE : VK_FALSE);
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_Y, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y ? VK_TRUE : VK_FALSE);
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_LEFT_SHOULDER, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ? VK_TRUE : VK_FALSE);
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_RIGHT_SHOULDER, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ? VK_TRUE : VK_FALSE);
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_BACK, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK ? VK_TRUE : VK_FALSE);
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_START, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_START ? VK_TRUE : VK_FALSE);
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_LEFT_THUMB, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB ? VK_TRUE : VK_FALSE);
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_RIGHT_THUMB, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ? VK_TRUE : VK_FALSE);
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_DPAD_LEFT, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ? VK_TRUE : VK_FALSE);
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_DPAD_RIGHT, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ? VK_TRUE : VK_FALSE);
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_DPAD_DOWN, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ? VK_TRUE : VK_FALSE);
                g_allWindows.valueAt(index)->getGamepadInput(i).setButton(VKTS_GAMEPAD_DPAD_UP, xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP ? VK_TRUE : VK_FALSE);

                axisValue = glm::clamp((float)xinputState.Gamepad.sThumbLX / 32767, -1.0f, 1.0f);
                g_allWindows.valueAt(index)->getGamepadInput(i).setAxis(VKTS_GAMEPAD_LEFT_STICK_X, axisValue);

                axisValue = glm::clamp((float)xinputState.Gamepad.sThumbLY / 32767, -1.0f, 1.0f);
                g_allWindows.valueAt(index)->getGamepadInput(i).setAxis(VKTS_GAMEPAD_LEFT_STICK_Y, axisValue);

                axisValue = glm::clamp((float)xinputState.Gamepad.sThumbRX / 32767, -1.0f, 1.0f);
                g_allWindows.valueAt(index)->getGamepadInput(i).setAxis(VKTS_GAMEPAD_RIGHT_STICK_X, axisValue);

                axisValue = glm::clamp((float)xinputState.Gamepad.sThumbRY / 32767, -1.0f, 1.0f);
                g_allWindows.valueAt(index)->getGamepadInput(i).setAxis(VKTS_GAMEPAD_RIGHT_STICK_Y, axisValue);

                axisValue = (float)xinputState.Gamepad.bLeftTrigger / 255.0f;
                g_allWindows.valueAt(index)->getGamepadInput(i).setAxis(VKTS_GAMEPAD_LEFT_TRIGGER, axisValue);

                axisValue = (float)xinputState.Gamepad.bRightTrigger / 255.0f;
                g_allWindows.valueAt(index)->getGamepadInput(i).setAxis(VKTS_GAMEPAD_RIGHT_TRIGGER, axisValue);
            }
        }
    }

    //

    for (size_t i = 0; i < g_allWindows.values().size(); i++)
    {
        if (g_allWindows.valueAt(i)->isInvisibleCursor())
        {
            POINT centerPosition = {(LONG)g_allWindows.valueAt(i)->getDimension().x / 2, (LONG)g_allWindows.valueAt(i)->getDimension().y / 2};

            ClientToScreen(g_allWindows.valueAt(i)->getNativeWindow(), &centerPosition);

            SetCursorPos(centerPosition.x, centerPosition.y);

            break;
        }
    }

    return VK_TRUE;
}

//

int32_t VKTS_APIENTRY _visualGetNumberDisplays()
{
    return static_cast<int32_t>(GetSystemMetrics(SM_CMONITORS));
}

INativeDisplayWP VKTS_APIENTRY _visualCreateDefaultDisplay()
{
    if (g_defaultDisplay.get())
    {
        return g_defaultDisplay;
    }

    int32_t displayIndex = 0;

    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);

    while (EnumDisplayDevices(NULL, displayIndex, &dd, 0))
    {
        if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
        {
            if (g_allDisplays.find(displayIndex) != g_allDisplays.end())
            {
                return INativeDisplayWP();
            }

            DEVMODE dm;
            dm.dmSize = sizeof(DEVMODE);

            if (!EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm))
            {
                return INativeDisplayWP();
            }

            int32_t width = static_cast<int32_t>(dm.dmPelsWidth);
            int32_t height = static_cast<int32_t>(dm.dmPelsHeight);

            HINSTANCE nativeDisplay = GetModuleHandle(NULL);

            if (!nativeDisplay)
            {
                return INativeDisplayWP();
            }

            auto currentDisplay = NativeDisplaySP(new NativeDisplay(displayIndex, VK_TRUE, width, height, nativeDisplay));

            if (!currentDisplay.get())
            {
                return INativeDisplayWP();
            }

            //

            VKTS_NATIVE_DISPLAY_CONTAINER* currentDisplayContainer = new VKTS_NATIVE_DISPLAY_CONTAINER();

            if (!currentDisplayContainer)
            {
                _visualDestroyDisplay(currentDisplay);

                return INativeDisplayWP();
            }

            strcpy(currentDisplayContainer->deviceName, dd.DeviceName);
            currentDisplayContainer->display = currentDisplay;
            currentDisplayContainer->width = width;
            currentDisplayContainer->height = height;

            g_allDisplays[currentDisplay->getIndex()] = currentDisplayContainer;

            //

            g_defaultDisplay = currentDisplay;

            return currentDisplay;
        }

        displayIndex++;
    }

    return INativeDisplayWP();
}

INativeDisplayWP VKTS_APIENTRY _visualCreateDisplay(const int32_t displayIndex)
{
    if (displayIndex < 0 || displayIndex >= _visualGetNumberDisplays())
    {
        return INativeDisplayWP();
    }

    auto walkerDisplayContainer = g_allDisplays.find(displayIndex);

    if (walkerDisplayContainer != g_allDisplays.end())
    {
        if (walkerDisplayContainer->second)
        {
            return walkerDisplayContainer->second->display;
        }

        return INativeDisplayWP();
    }

    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);

    if (!EnumDisplayDevices(NULL, displayIndex, &dd, 0))
    {
        return INativeDisplayWP();
    }

    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);

    if (!EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm))
    {
        return INativeDisplayWP();
    }

    int32_t width = static_cast<int32_t>(dm.dmPelsWidth);
    int32_t height = static_cast<int32_t>(dm.dmPelsHeight);

    HINSTANCE nativeDisplay = GetModuleHandle(NULL);

    if (!nativeDisplay)
    {
        return INativeDisplayWP();
    }

    auto currentDisplay = NativeDisplaySP(new NativeDisplay(displayIndex, dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE, width, height, nativeDisplay));

    if (!currentDisplay.get())
    {
        return INativeDisplayWP();
    }

    //

    VKTS_NATIVE_DISPLAY_CONTAINER* currentContainer = new VKTS_NATIVE_DISPLAY_CONTAINER();

    if (!currentContainer)
    {
        _visualDestroyDisplay(currentDisplay);

        return INativeDisplayWP();
    }

    strcpy(currentContainer->deviceName, dd.DeviceName);
    currentContainer->display = currentDisplay;
    currentContainer->width = width;
    currentContainer->height = height;

    g_allDisplays[currentDisplay->getIndex()] = currentContainer;

    //

    if (currentDisplay->isDefault())
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
        delete walkerDisplayContainer->second;

        walkerDisplayContainer->second = nullptr;
    }

    g_allDisplays.erase(walkerDisplayContainer);
}

//

INativeWindowWP VKTS_APIENTRY _visualCreateWindow(const INativeDisplayWP& display, const char* title, const int32_t width, const int32_t height, const VkBool32 fullscreen, const VkBool32 resize, const VkBool32 invisibleCursor)
{
    auto sharedDisplay = display.lock();

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

    if (!sharedDisplay->getNativeDisplay())
    {
        logPrint(VKTS_LOG_ERROR, "Visual: No native display.");

        return INativeWindowWP();
    }

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

    VkBool32 finalResize = resize && !fullscreen;

    //

    WNDCLASS wc;
    RECT wRect;
    HINSTANCE hInstance;

    DWORD dwStyle, dwExStyle;

    dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE;
    dwExStyle = WS_EX_APPWINDOW;

    if (fullscreen)
    {
        auto& currentDisplayContainer = walkerDisplayContainer->second;

        if (!currentDisplayContainer)
        {
            logPrint(VKTS_LOG_ERROR, "Visual: Could not find display.");

            return INativeWindowWP();
        }

        //

        DEVMODE dm;
        dm.dmSize = sizeof(DEVMODE);

        uint32_t modeNumber = 0;
        uint32_t displayFrequency = 0;
        bool found = false;

        while (EnumDisplaySettings(currentDisplayContainer->deviceName, modeNumber, &dm))
        {
            if (dm.dmPelsWidth == (DWORD)width && dm.dmPelsHeight == (DWORD)height)
            {
                found = true;

                if (dm.dmDisplayFrequency > displayFrequency)
                {
                    displayFrequency = dm.dmDisplayFrequency;
                }
            }

            modeNumber++;
        }

        if (!found)
        {
            logPrint(VKTS_LOG_ERROR, "Visual: Could not find full screen mode.");

            return INativeWindowWP();
        }

        dm.dmPelsWidth = width;
        dm.dmPelsHeight = height;
        dm.dmDisplayFrequency = displayFrequency;
        dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

        if (ChangeDisplaySettingsEx(currentDisplayContainer->deviceName, &dm, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
        {
            logPrint(VKTS_LOG_ERROR, "Visual: Could not switch to full screen.");

            return INativeWindowWP();
        }

        dwStyle |= WS_POPUP;
    }
    else
    {
        dwStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

        if (finalResize)
        {
            dwStyle |= WS_MAXIMIZEBOX | WS_SIZEBOX;
            dwExStyle |= WS_EX_WINDOWEDGE;
        }
    }

    hInstance = GetModuleHandle(NULL);

    // Check, if class already registered.
    if (!GetClassInfo(hInstance, VKTS_WINDOWS_CLASSNAME, &wc))
    {
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = (WNDPROC)_visualProcessMessages;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = NULL;
        wc.lpszMenuName = NULL;
        wc.lpszClassName = VKTS_WINDOWS_CLASSNAME;

        RegisterClass(&wc);
    }

    //

    wRect.left = 0L;
    wRect.right = (LONG)width - 1;
    wRect.top = 0L;
    wRect.bottom = (LONG)height - 1;

    AdjustWindowRectEx(&wRect, dwStyle, FALSE, dwExStyle);

    // Set window on correct screen.

    int32_t xoffset = 0;
    int32_t yoffset = 0;

    int32_t displayIndex = 0;

    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);

    while (EnumDisplayDevices(NULL, displayIndex, &dd, 0))
    {
        if (displayIndex == sharedDisplay->getIndex())
        {
            DEVMODE dm;
            dm.dmSize = sizeof(DEVMODE);

            if (!EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm))
            {
                displayIndex++;

                continue;
            }

            xoffset = dm.dmPosition.x;
            yoffset = dm.dmPosition.y;

            break;
        }

        displayIndex++;
    }

    //

    HWND nativeWindow = CreateWindowEx(dwExStyle, VKTS_WINDOWS_CLASSNAME, title, dwStyle, xoffset, yoffset, wRect.right - wRect.left + 1, wRect.bottom - wRect.top + 1, NULL, NULL, hInstance, NULL);

    ShowWindow(nativeWindow, SW_SHOW);
    SetForegroundWindow(nativeWindow);
    SetFocus(nativeWindow);

    if (invisibleCursor)
    {
        while (ShowCursor(FALSE) >= 0);
    }

    //

    auto currentWindow = NativeWindowSP(new NativeWindow(display, nativeWindow, currentWindowIndex, title, width, height, fullscreen, finalResize, invisibleCursor));

    if (!currentWindow.get())
    {
        logPrint(VKTS_LOG_ERROR, "Visual: Could not create native window.");

        char className[VKTS_WINDOWS_MAX_CHARS + 1];

        int result = GetClassName(nativeWindow, className, VKTS_WINDOWS_MAX_CHARS);

        if (invisibleCursor)
        {
            while (ShowCursor(TRUE) < 0);
        }

        DestroyWindow(nativeWindow);

        if (result)
        {
            UnregisterClass(className, NULL);
        }

        if (fullscreen)
        {
            ChangeDisplaySettingsEx(walkerDisplayContainer->second->deviceName, NULL, NULL, 0, NULL);

            if (walkerDisplayContainer->second->display.get())
            {
                walkerDisplayContainer->second->display->setDimension(glm::ivec2(walkerDisplayContainer->second->width, walkerDisplayContainer->second->height));
            }
        }

        return INativeWindowWP();
    }

    if (g_allWindows.find(nativeWindow) != g_allWindows.size())
    {
        logPrint(VKTS_LOG_ERROR, "Visual: Native window already in list.");

        _visualDestroyWindow(currentWindow);

        return INativeWindowWP();
    }

    g_allWindows[nativeWindow] = currentWindow;

    //

    g_windowBits |= currentWindowBit;

    //

    if (fullscreen)
    {
        walkerDisplayContainer->second->display->setDimension(glm::ivec2(width, height));
    }

    //

    // If resolution changed, adjust all - not just for this - window positions.

    if (fullscreen)
    {
        displayIndex = 0;

        while (EnumDisplayDevices(NULL, displayIndex, &dd, 0))
        {
            DEVMODE dm;
            dm.dmSize = sizeof(DEVMODE);

            if (!EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm))
            {
                displayIndex++;

                continue;
            }

            xoffset = dm.dmPosition.x;
            yoffset = dm.dmPosition.y;

            for (size_t index = 0; index < g_allWindows.size(); index++)
            {
                if (g_allWindows.valueAt(index).get())
                {
                    auto sharedDisplayWindow = g_allWindows.valueAt(index)->getNativeDisplay().lock();

                    if (sharedDisplayWindow.get())
                    {
                        if (displayIndex == sharedDisplayWindow->getIndex())
                        {
                            SetWindowPos(g_allWindows.keyAt(index), HWND_TOP, xoffset, yoffset, g_allWindows.valueAt(index)->getWidth(), g_allWindows.valueAt(index)->getHeight(), SWP_NOZORDER | SWP_NOSIZE);
                        }
                    }
                }
            }

            displayIndex++;
        }
    }

    return currentWindow;
}

NativeWindowSP VKTS_APIENTRY _visualGetWindow(const int32_t windowIndex)
{
    for (size_t index = 0; index < g_allWindows.size(); index++)
    {
        if (g_allWindows.valueAt(index).get())
        {
            if (g_allWindows.valueAt(index).get()->getIndex() == windowIndex)
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
        HWND nativeWindow = window->getNativeWindow();

        char className[VKTS_WINDOWS_MAX_CHARS + 1];

        int result = GetClassName(nativeWindow, className, VKTS_WINDOWS_MAX_CHARS);

        if (window->isInvisibleCursor())
        {
            while (ShowCursor(TRUE) < 0);
        }

        DestroyWindow(nativeWindow);

        if (result)
        {
            UnregisterClass(className, NULL);
        }
    }

    uint32_t currentNegativeWindowBit = ~(1 << window->getIndex());

    g_windowBits &= currentNegativeWindowBit;

    //

    auto sharedDisplay = window->getNativeDisplay().lock();

    if (window->isFullscreen() && sharedDisplay.get())
    {
        auto walkerDisplayContainer = g_allDisplays.find(sharedDisplay->getIndex());

        if (walkerDisplayContainer != g_allDisplays.end())
        {
            if (walkerDisplayContainer->second)
            {
                ChangeDisplaySettingsEx(walkerDisplayContainer->second->deviceName, NULL, NULL, 0, NULL);

                if (walkerDisplayContainer->second->display.get())
                {
                    walkerDisplayContainer->second->display->setDimension(glm::ivec2(walkerDisplayContainer->second->width, walkerDisplayContainer->second->height));
                }
            }
        }
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

    auto walkerDisplayContainer = g_allDisplays.begin();
    while (walkerDisplayContainer != g_allDisplays.end())
    {
        if (walkerDisplayContainer->second)
        {
            _visualDestroyDisplay(walkerDisplayContainer->second->display);
        }

        walkerDisplayContainer = g_allDisplays.begin();
    }
    g_allDisplays.clear();

    g_defaultDisplay.reset();

    // Terminate gamepad not needed.
}

}
