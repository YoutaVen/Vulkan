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

#ifndef VKTS_GAMEPADINPUT_HPP_
#define VKTS_GAMEPADINPUT_HPP_

#include <vkts/vkts.hpp>

#define VKTS_AXIS_DEAD_ZONE 0.15f

namespace vkts
{

class GamepadInput
{

private:

    VkBool32 buttons[VKTS_MAX_GAMEPAD_BUTTONS];

    mutable std::mutex mutexButtons;

    float axis[VKTS_MAX_GAMEPAD_AXIS];

    mutable std::mutex mutexAxis;

public:

    GamepadInput();
    GamepadInput(const GamepadInput& other) = delete;
    GamepadInput(GamepadInput&& other) = delete;
    virtual ~GamepadInput();

    GamepadInput& operator =(const GamepadInput& other) = delete;

    GamepadInput& operator =(GamepadInput && other) = delete;

    //

    VkBool32 getButton(const int32_t buttonIndex) const;

    void setButton(const int32_t buttonIndex, const VkBool32 pressed);

    //

    float getAxis(const int32_t axisIndex) const;

    void setAxis(const int32_t axisIndex, const float value);

    //

    void resetGamepad();

};

} /* namespace vkts */

#endif /* VKTS_GAMEPADINPUT_HPP_ */
