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

#ifndef VKTS_MOUSEINPUT_HPP_
#define VKTS_MOUSEINPUT_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class MouseInput
{

private:

    VkBool32 buttons[VKTS_MAX_MOUSE_BUTTONS];

    mutable std::mutex mutexButtons;

    glm::ivec2 location;

    mutable std::mutex mutexLocation;

    int32_t mouseWheel;

    mutable std::mutex mutexMouseWheel;

public:

    MouseInput();
    MouseInput(const MouseInput& other) = delete;
    MouseInput(MouseInput&& other) = delete;
    virtual ~MouseInput();

    MouseInput& operator =(const MouseInput& other) = delete;

    MouseInput& operator =(MouseInput && other) = delete;

    //

    VkBool32 getButton(const int32_t buttonIndex) const;

    void setButton(const int32_t buttonIndex, const VkBool32 pressed);

    //

    const glm::ivec2& getLocation() const;

    void setLocation(const glm::ivec2& location);

    //

    int32_t getMouseWheel() const;

    void setMouseWheel(const int32_t mosueWheel);

    //

    void resetMouse();

};

} /* namespace vkts */

#endif /* VKTS_MOUSEINPUT_HPP_ */
