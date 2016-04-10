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

#include "GamepadInput.hpp"

namespace vkts
{

GamepadInput::GamepadInput() :
    mutexButtons(), mutexAxis()
{
    resetGamepad();
}

GamepadInput::~GamepadInput()
{

}

VkBool32 GamepadInput::getButton(const int32_t buttonIndex) const
{
    if (buttonIndex < 0 || buttonIndex >= VKTS_MAX_GAMEPAD_BUTTONS)
    {
        return VK_FALSE;
    }

    std::lock_guard<std::mutex> lockGuard(mutexButtons);

    return buttons[buttonIndex];
}

void GamepadInput::setButton(const int32_t buttonIndex, const VkBool32 pressed)
{
    if (buttonIndex < 0 || buttonIndex >= VKTS_MAX_GAMEPAD_BUTTONS)
    {
        return;
    }

    std::lock_guard<std::mutex> lockGuard(mutexButtons);

    buttons[buttonIndex] = pressed;
}

float GamepadInput::getAxis(const int32_t axisIndex) const
{
    if (axisIndex < 0 || axisIndex >= VKTS_MAX_GAMEPAD_AXIS)
    {
        return 0.0f;
    }

    std::lock_guard<std::mutex> lockGuard(mutexAxis);

    return axis[axisIndex];
}

void GamepadInput::setAxis(const int32_t axisIndex, const float value)
{
    if (axisIndex < 0 || axisIndex >= VKTS_MAX_GAMEPAD_AXIS)
    {
        return;
    }

    std::lock_guard<std::mutex> lockGuard(mutexAxis);

    if (value < VKTS_AXIS_DEAD_ZONE && value > -VKTS_AXIS_DEAD_ZONE)
    {
        axis[axisIndex] = 0.0f;
    }
    else if (value > 0.0f)
    {
        axis[axisIndex] = (value - VKTS_AXIS_DEAD_ZONE)
                          / (1.0f - VKTS_AXIS_DEAD_ZONE);
    }
    else if (value < 0.0f)
    {
        axis[axisIndex] = (value + VKTS_AXIS_DEAD_ZONE)
                          / (1.0f - VKTS_AXIS_DEAD_ZONE);
    }
    else
    {
        axis[axisIndex] = 0.0f;
    }
}

void GamepadInput::resetGamepad()
{
    std::lock_guard<std::mutex> lockGuardButtons(mutexButtons);
    std::lock_guard<std::mutex> lockGuardMouseWheel(mutexAxis);

    for (int32_t mouseButtonIndex = 0;
            mouseButtonIndex < VKTS_MAX_GAMEPAD_BUTTONS; mouseButtonIndex++)
    {
        buttons[mouseButtonIndex] = VK_FALSE;
    }

    for (int32_t axisIndex = 0; axisIndex < VKTS_MAX_GAMEPAD_AXIS; axisIndex++)
    {
        axis[axisIndex] = 0.0f;
    }
}

} /* namespace vkts */
