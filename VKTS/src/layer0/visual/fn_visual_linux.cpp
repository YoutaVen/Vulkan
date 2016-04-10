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

#include "fn_visual_linux_internal.hpp"

namespace vkts
{

static int joystickFile[VKTS_MAX_GAMEPADS];

//

VkBool32 VKTS_APIENTRY _visualInitGamepad(const VkInstance instance, const VkPhysicalDevice physicalDevice)
{
	int32_t gamepadIndex = 0;

    for (gamepadIndex = 0; gamepadIndex < VKTS_MAX_GAMEPADS; gamepadIndex++)
    {
        joystickFile[gamepadIndex] = -1;
    }

    gamepadIndex = 0;

    //

    char joystickDeviceBuffer[VKTS_MAX_BUFFER_CHARS + 1];

    int file = -1;

    int32_t queryIndex = 0;

    do {
        sprintf(joystickDeviceBuffer, "/dev/input/js%d", queryIndex);

        file = open(joystickDeviceBuffer, O_RDONLY | O_NONBLOCK);

        if (file != -1)
        {
    		char numberAxes;
    		char numberButtons;

    		if (ioctl(file, JSIOCGAXES, &numberAxes) >= 0 && ioctl(file, JSIOCGBUTTONS, &numberButtons) >= 0)
        	{
    			if (numberAxes >= 8 && numberButtons >= 11)
    			{
    				joystickFile[gamepadIndex] = file;

    				gamepadIndex++;
    			}
            	else
            	{
            		close(file);
            	}
        	}
        	else
        	{
        		close(file);
        	}
        }

        queryIndex++;

    } while (file != -1 && gamepadIndex < VKTS_MAX_GAMEPADS);

    return VK_TRUE;
}

VkBool32 VKTS_APIENTRY _visualDispatchMessagesGamepad()
{
    struct js_event joystickEvent;

    ssize_t bytesRead;

    for (int32_t i = 0; i < VKTS_MAX_GAMEPADS; i++)
    {
        if (joystickFile[i] != -1)
        {
            bytesRead = read(joystickFile[i], &joystickEvent, sizeof(joystickEvent));

            if (bytesRead == -1 || bytesRead != sizeof(joystickEvent))
            {
                continue;
            }

            int32_t buttonIndex = -1;
            VkBool32 buttonPressed;

            int32_t axisIndex = -1;
            float axisValue = 0.0f;

            if (joystickEvent.type & JS_EVENT_BUTTON)
            {
                buttonPressed = joystickEvent.value == 0 ? VK_FALSE : VK_TRUE;

                switch (joystickEvent.number)
                {
                    case 0:
                        buttonIndex = VKTS_GAMEPAD_A;
                        break;
                    case 1:
                        buttonIndex = VKTS_GAMEPAD_B;
                        break;
                    case 2:
                        buttonIndex = VKTS_GAMEPAD_X;
                        break;
                    case 3:
                        buttonIndex = VKTS_GAMEPAD_Y;
                        break;
                    case 4:
                        buttonIndex = VKTS_GAMEPAD_LEFT_SHOULDER;
                        break;
                    case 5:
                        buttonIndex = VKTS_GAMEPAD_RIGHT_SHOULDER;
                        break;
                    case 6:
                        buttonIndex = VKTS_GAMEPAD_BACK;
                        break;
                    case 7:
                        buttonIndex = VKTS_GAMEPAD_START;
                        break;
                    case 9:
                        buttonIndex = VKTS_GAMEPAD_LEFT_THUMB;
                        break;
                    case 10:
                        buttonIndex = VKTS_GAMEPAD_RIGHT_THUMB;
                        break;
                }
            }

            if (joystickEvent.type & JS_EVENT_AXIS)
            {
                axisValue = static_cast<const float>(joystickEvent.value) / 32767.0f;

                switch (joystickEvent.number)
                {
                    case 0:
                        axisIndex = VKTS_GAMEPAD_LEFT_STICK_X;
                        break;
                    case 1:
                        axisIndex = VKTS_GAMEPAD_LEFT_STICK_Y;

                        axisValue = -axisValue;

                        break;
                    case 2:
                        axisIndex = VKTS_GAMEPAD_LEFT_TRIGGER;

                        axisValue = glm::clamp((axisValue + 1.0f) * 0.5f, 0.0f, 1.0f);
                        break;
                    case 3:
                        axisIndex = VKTS_GAMEPAD_RIGHT_STICK_X;
                        break;
                    case 4:
                        axisIndex = VKTS_GAMEPAD_RIGHT_STICK_Y;

                        axisValue = -axisValue;

                        break;
                    case 5:
                        axisIndex = VKTS_GAMEPAD_RIGHT_TRIGGER;

                        axisValue = glm::clamp((axisValue + 1.0f) * 0.5f, 0.0f, 1.0f);
                        break;
                    case 6:
                    {
                        buttonPressed = axisValue < 0.0f ? VK_TRUE : VK_FALSE;

                        _visualGamepadSetButton(i, VKTS_GAMEPAD_DPAD_LEFT, buttonPressed);

                        buttonPressed = axisValue > 0.0f ? VK_TRUE : VK_FALSE;

                        _visualGamepadSetButton(i, VKTS_GAMEPAD_DPAD_RIGHT, buttonPressed);
                    }
                    break;
                    case 7:
                    {
                        buttonPressed = axisValue > 0.0f ? VK_TRUE : VK_FALSE;

                        _visualGamepadSetButton(i, VKTS_GAMEPAD_DPAD_DOWN, buttonPressed);

                        buttonPressed = axisValue < 0.0f ? VK_TRUE : VK_FALSE;

                        _visualGamepadSetButton(i, VKTS_GAMEPAD_DPAD_UP, buttonPressed);
                    }
                    break;
                }

            }

            if (buttonIndex != -1)
            {
            	_visualGamepadSetButton(i, buttonIndex, buttonPressed);
            }

            if (axisIndex != -1)
            {
            	_visualGamepadSetAxis(i, axisIndex, axisValue);
            }
        }
    }

    return VK_TRUE;
}

void VKTS_APIENTRY _visualTerminateGamepad()
{
    for (int32_t i = 0; i < VKTS_MAX_GAMEPADS; i++)
    {
        if (joystickFile[i] != -1)
        {
            close(joystickFile[i]);

            joystickFile[i] = -1;
        }
    }
}

}
