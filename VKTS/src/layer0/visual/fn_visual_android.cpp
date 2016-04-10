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

#include "fn_visual_android_internal.hpp"

#include "../input/GamepadInput.hpp"
#include "../input/KeyInput.hpp"
#include "../input/MouseInput.hpp"

#define VKTS_WINDOWS_MAX_DISPLAYS 1

#define VKTS_WINDOWS_MAX_WINDOWS 1

//

extern int main(int argc, char* argv[]);

static struct android_app* g_app = nullptr;

static ANativeWindow* g_nativeAndroidWindow = nullptr;
static int32_t g_width = -1;
static int32_t g_height = -1;

//

static vkts::NativeDisplaySP g_defaultDisplay;

static vkts::NativeWindowSP g_defaultWindow;

//

const struct android_app* VKTS_APIENTRY _visualGetAndroidApp()
{
	return g_app;
}

//

static void _onConfigurationChanged(ANativeActivity* ac)
{
    // Do nothing, but the callback has to be called.
}

static void _onAppCmd(struct android_app* app, int32_t cmd)
{
    if (!app || app != g_app)
    {
        return;
    }

    switch (cmd)
    {
        case APP_CMD_INIT_WINDOW:
        {
            g_width = ANativeWindow_getWidth(g_app->window);
            g_height = ANativeWindow_getHeight(g_app->window);

            g_nativeAndroidWindow = app->window;
        }
        break;

        case APP_CMD_TERM_WINDOW:
        case APP_CMD_DESTROY:

            g_app->destroyRequested = 1;

            break;
    }
}

static int32_t _onInputEvent(struct android_app* app, AInputEvent* event)
{
    if (!event || !::g_defaultWindow.get())
    {
        return 1;
    }

    int32_t source = AInputEvent_getSource(event);

    if (source == AINPUT_SOURCE_MOUSE)
    {
        int32_t x = (int32_t)AMotionEvent_getX(event, 0);
        int32_t y = (int32_t)AMotionEvent_getY(event, 0);

        g_defaultWindow->getMouseInput().setLocation(glm::ivec2(x, y));


        int32_t buttonState = AMotionEvent_getButtonState(event);

        g_defaultWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_LEFT, buttonState & AMOTION_EVENT_BUTTON_PRIMARY);
        g_defaultWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_RIGHT, buttonState & AMOTION_EVENT_BUTTON_SECONDARY);
        g_defaultWindow->getMouseInput().setButton(VKTS_MOUSE_BUTTON_MIDDLE, buttonState & AMOTION_EVENT_BUTTON_TERTIARY);


        int32_t vscroll = (int32_t)AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_VSCROLL, 0);

        g_defaultWindow->getMouseInput().setMouseWheel(vscroll);

        return 1;
    }
    else if (source == AINPUT_SOURCE_KEYBOARD)
    {
        VkBool32 pressed = (AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_DOWN);

        int32_t scanCode = AKeyEvent_getScanCode(event);

        int32_t key = vkts::_visualTranslateKey(scanCode);

        if (pressed && key == VKTS_KEY_ESCAPE)
        {
            g_app->destroyRequested = 1;
        }
        else if (key != VKTS_KEY_UNKNOWN)
        {
            g_defaultWindow->getKeyInput().setKey(key, pressed);
        }

        return 1;
    }
    else if (source == AINPUT_SOURCE_JOYSTICK)
    {
        int32_t pointerIndex = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

        if (pointerIndex >= 0 && pointerIndex < VKTS_MAX_GAMEPADS)
        {
            for (int32_t axis = AMOTION_EVENT_AXIS_X; axis <= AMOTION_EVENT_AXIS_RTRIGGER; axis++)
            {
                int32_t axisIndex = -1;

                float axisValue = AMotionEvent_getAxisValue(event, axis, pointerIndex);

                switch (axis)
                {
                    case AMOTION_EVENT_AXIS_X:
                        axisIndex = VKTS_GAMEPAD_LEFT_STICK_X;
                        break;
                    case AMOTION_EVENT_AXIS_Y:
                        axisIndex = VKTS_GAMEPAD_LEFT_STICK_Y;
                        axisValue = -axisValue;
                        break;
                    case AMOTION_EVENT_AXIS_Z:
                        axisIndex = VKTS_GAMEPAD_RIGHT_STICK_X;
                        break;
                    case AMOTION_EVENT_AXIS_RZ:
                        axisIndex = VKTS_GAMEPAD_RIGHT_STICK_Y;
                        axisValue = -axisValue;
                        break;
                    case AMOTION_EVENT_AXIS_LTRIGGER:
                        axisIndex = VKTS_GAMEPAD_LEFT_TRIGGER;
                        break;
                    case AMOTION_EVENT_AXIS_RTRIGGER:
                        axisIndex = VKTS_GAMEPAD_RIGHT_TRIGGER;
                        break;
                    case AMOTION_EVENT_AXIS_HAT_X:
                    {
                        VkBool32 buttonPressed = axisValue < 0.0f ? VK_TRUE : VK_FALSE;

                        g_defaultWindow->getGamepadInput(pointerIndex).setButton(VKTS_GAMEPAD_DPAD_LEFT, buttonPressed);

                        buttonPressed = axisValue > 0.0f ? VK_TRUE : VK_FALSE;

                        g_defaultWindow->getGamepadInput(pointerIndex).setButton(VKTS_GAMEPAD_DPAD_RIGHT, buttonPressed);
                    }
                    break;
                    case AMOTION_EVENT_AXIS_HAT_Y:
                    {
                        VkBool32 buttonPressed = axisValue > 0.0f ? VK_TRUE : VK_FALSE;

                        g_defaultWindow->getGamepadInput(pointerIndex).setButton(VKTS_GAMEPAD_DPAD_DOWN, buttonPressed);

                        buttonPressed = axisValue < 0.0f ? VK_TRUE : VK_FALSE;

                        g_defaultWindow->getGamepadInput(pointerIndex).setButton(VKTS_GAMEPAD_DPAD_UP, buttonPressed);
                    }
                    break;
                }

                if (axisIndex == -1)
                {
                    continue;
                }

                g_defaultWindow->getGamepadInput(pointerIndex).setAxis(axisIndex, axisValue);
            }
        }

        return 1;
    }
    else if (source == (AINPUT_SOURCE_GAMEPAD | AINPUT_SOURCE_KEYBOARD))
    {
        int32_t pointerIndex = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

        if (pointerIndex >= 0 && pointerIndex < VKTS_MAX_GAMEPADS)
        {
            VkBool32 buttonPressed = (AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_DOWN);

            int32_t keyCode = AKeyEvent_getKeyCode(event);

            int32_t buttonIndex = -1;

            switch (keyCode)
            {
                case AKEYCODE_BUTTON_A:
                    buttonIndex = VKTS_GAMEPAD_A;
                    break;
                case AKEYCODE_BUTTON_B:
                    buttonIndex = VKTS_GAMEPAD_B;
                    break;
                case AKEYCODE_BUTTON_X:
                    buttonIndex = VKTS_GAMEPAD_X;
                    break;
                case AKEYCODE_BUTTON_Y:
                    buttonIndex = VKTS_GAMEPAD_Y;
                    break;
                case AKEYCODE_BUTTON_L1:
                    buttonIndex = VKTS_GAMEPAD_LEFT_SHOULDER;
                    break;
                case AKEYCODE_BUTTON_R1:
                    buttonIndex = VKTS_GAMEPAD_RIGHT_SHOULDER;
                    break;
                case AKEYCODE_BACK:
                    buttonIndex = VKTS_GAMEPAD_BACK;
                    break;
                case AKEYCODE_BUTTON_START:
                    buttonIndex = VKTS_GAMEPAD_START;
                    break;
                case AKEYCODE_BUTTON_THUMBL:
                    buttonIndex = VKTS_GAMEPAD_LEFT_THUMB;
                    break;
                case AKEYCODE_BUTTON_THUMBR:
                    buttonIndex = VKTS_GAMEPAD_RIGHT_THUMB;
                    break;
            }

            if (buttonIndex != -1)
            {
                g_defaultWindow->getGamepadInput(pointerIndex).setButton(buttonIndex, buttonPressed);
            }
        }

        return 1;
    }

    return 1;
}


void android_main(struct android_app* app)
{
    // Make sure glue isn't stripped.
    app_dummy();

    if (!app)
    {
        ::exit(-1);
    }

    //

    g_app = app;

    g_app->onAppCmd = _onAppCmd;
    g_app->onInputEvent = _onInputEvent;

    g_app->activity->callbacks->onConfigurationChanged = _onConfigurationChanged;

    //

    // Set data path.
    vkts::_fileSetBaseDirectory(g_app->activity->externalDataPath);

    //

    // Force fullscreen.
    ANativeActivity_setWindowFlags(g_app->activity, AWINDOW_FLAG_FULLSCREEN, 0);

    //

    VkBool32 doInit = VK_TRUE;

    while (doInit)
    {
        int ident;
        int events;
        struct android_poll_source* androidPollSource;

        while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&androidPollSource)) >= 0)
        {
            if (androidPollSource != NULL)
            {
                androidPollSource->process(g_app, androidPollSource);
            }
        }

        if (g_app->destroyRequested != 0)
        {
            ::exit(0);
        }

        if (g_nativeAndroidWindow != nullptr)
        {
            doInit = VK_FALSE;
        }
    }

    //

    main(0, nullptr);

    //

    g_width = -1;
    g_height = -1;

    g_nativeAndroidWindow = nullptr;

    g_app = nullptr;

    //

    ::exit(0);
}

//

namespace vkts
{

VkBool32 VKTS_APIENTRY _visualInit(const VkInstance instance, const VkPhysicalDevice physicalDevice)
{
    if (g_app == nullptr || g_app->window == nullptr || g_width == -1 || g_height == -1)
    {
        return VK_FALSE;
    }

    //

    ::g_defaultDisplay = NativeDisplaySP(new NativeDisplay(0, VK_TRUE, g_width, g_height, nullptr));

    if (!::g_defaultDisplay.get())
    {
        return VK_FALSE;
    }

    ::g_defaultWindow = NativeWindowSP(new NativeWindow(::g_defaultDisplay, g_app->window, 0, "", g_width, g_height, VK_TRUE, VK_TRUE, VK_FALSE));

    if (!::g_defaultWindow.get())
    {
        return VK_FALSE;
    }

    return _visualInitKey();
}

VkBool32 VKTS_APIENTRY _visualDispatchMessages()
{
    int ident;
    int events;
    struct android_poll_source* androidPollSource;

    while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&androidPollSource)) >= 0)
    {
        if (androidPollSource != NULL)
        {
            androidPollSource->process(g_app, androidPollSource);
        }
    }

    int32_t width = ANativeWindow_getWidth(g_app->window);
    int32_t height = ANativeWindow_getHeight(g_app->window);

    if (width != g_width || height != g_height)
    {
        g_width = width;
        g_height = height;

        if (::g_defaultWindow.get())
        {
            ::g_defaultWindow->setDimension(glm::ivec2(g_width, g_height));
        }
    }

    if (g_app->destroyRequested != 0)
    {
        return VK_FALSE;
    }

    return VK_TRUE;
}

//

int32_t VKTS_APIENTRY _visualGetNumberDisplays()
{
    return VKTS_WINDOWS_MAX_DISPLAYS;
}

INativeDisplayWP VKTS_APIENTRY _visualCreateDefaultDisplay()
{
    return ::g_defaultDisplay;
}

INativeDisplayWP VKTS_APIENTRY _visualCreateDisplay(const int32_t displayIndex)
{
    if (displayIndex < 0 || displayIndex >= _visualGetNumberDisplays())
    {
        return INativeDisplayWP();
    }

    return ::g_defaultDisplay;
}

NativeDisplaySP VKTS_APIENTRY _visualGetDisplay(const int32_t displayIndex)
{
    if (displayIndex < 0 || displayIndex >= VKTS_WINDOWS_MAX_DISPLAYS)
    {
        return NativeDisplaySP();
    }

    return ::g_defaultDisplay;
}

const SmartPointerVector<NativeDisplaySP>& VKTS_APIENTRY _visualGetActiveDisplays()
{
    static SmartPointerVector<NativeDisplaySP> displayList;

    displayList.clear();

    if (::g_defaultDisplay.get())
    {
        displayList.append(::g_defaultDisplay);
    }

    return displayList;
}

void VKTS_APIENTRY _visualDestroyDisplay(const NativeDisplaySP& display)
{
    // Note: On Android, a display will be destroyed at the end.
}

//

INativeWindowWP VKTS_APIENTRY _visualCreateWindow(const INativeDisplayWP& display, const char* title, const int32_t width, const int32_t height, const VkBool32 fullscreen, const VkBool32 resize, const VkBool32 invisibleCursor)
{
    // Note: On Android, a window cannot be created and the existing one has to be used.

    return INativeWindowWP();
}

NativeWindowSP VKTS_APIENTRY _visualGetWindow(const int32_t windowIndex)
{
    if (windowIndex < 0 || windowIndex >= VKTS_WINDOWS_MAX_WINDOWS)
    {
        return NativeWindowSP();
    }

    return ::g_defaultWindow;
}

const SmartPointerVector<NativeWindowSP>& VKTS_APIENTRY _visualGetActiveWindows()
{
    static SmartPointerVector<NativeWindowSP> windowList;

    windowList.clear();

    if (::g_defaultWindow.get())
    {
        windowList.append(::g_defaultWindow);
    }

    return windowList;
}

void VKTS_APIENTRY _visualDestroyWindow(const NativeWindowSP& window)
{
    // Note: On Android, a window will be destroyed at the end.
}

//

void VKTS_APIENTRY _visualTerminate()
{
    if (::g_defaultWindow.get())
    {
        ::g_defaultWindow->destroy();

        ::g_defaultWindow = NativeWindowSP();
    }

    if (::g_defaultDisplay.get())
    {
        ::g_defaultDisplay->destroy();

        ::g_defaultDisplay = NativeDisplaySP();
    }
}

}
