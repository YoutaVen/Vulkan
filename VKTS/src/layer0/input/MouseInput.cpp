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

#include "MouseInput.hpp"

namespace vkts
{

MouseInput::MouseInput() :
    mutexButtons(), mutexLocation(), mutexMouseWheel()
{
    resetMouse();
}

MouseInput::~MouseInput()
{

}

VkBool32 MouseInput::getButton(const int32_t buttonIndex) const
{
    if (buttonIndex < 0 || buttonIndex >= VKTS_MAX_MOUSE_BUTTONS)
    {
        return VK_FALSE;
    }

    std::lock_guard<std::mutex> lockGuard(mutexButtons);

    return buttons[buttonIndex];
}

void MouseInput::setButton(const int32_t buttonIndex, const VkBool32 pressed)
{
    if (buttonIndex < 0 || buttonIndex >= VKTS_MAX_MOUSE_BUTTONS)
    {
        return;
    }

    std::lock_guard<std::mutex> lockGuard(mutexButtons);

    buttons[buttonIndex] = pressed;
}

const glm::ivec2& MouseInput::getLocation() const
{
    std::lock_guard<std::mutex> lockGuard(mutexLocation);

    return location;
}

void MouseInput::setLocation(const glm::ivec2& location)
{
    std::lock_guard<std::mutex> lockGuard(mutexLocation);

    this->location = location;
}

int32_t MouseInput::getMouseWheel() const
{
    std::lock_guard<std::mutex> lockGuard(mutexMouseWheel);

    return mouseWheel;
}

void MouseInput::setMouseWheel(const int32_t mouseWheel)
{
    std::lock_guard<std::mutex> lockGuard(mutexMouseWheel);

    this->mouseWheel += mouseWheel;
}

void MouseInput::resetMouse()
{
    std::lock_guard<std::mutex> lockGuardButtons(mutexButtons);
    std::lock_guard<std::mutex> lockGuardLocation(mutexLocation);
    std::lock_guard<std::mutex> lockGuardMouseWheel(mutexMouseWheel);

    for (int32_t mouseButtonIndex = 0; mouseButtonIndex < VKTS_MAX_MOUSE_BUTTONS; mouseButtonIndex++)
    {
        buttons[mouseButtonIndex] = VK_FALSE;
    }

    location = glm::ivec2(-1, -1);

    mouseWheel = 0;
}

} /* namespace vkts */
