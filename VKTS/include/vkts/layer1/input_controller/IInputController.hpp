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

#ifndef VKTS_IINPUTCONTROLLER_HPP_
#define VKTS_IINPUTCONTROLLER_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class IInputController: public IUpdateable, public ICloneable<IInputController>
{

public:

    IInputController() :
        IUpdateable(), ICloneable()
    {
    }

    virtual ~IInputController()
    {
    }

    virtual int32_t getWindowIndex() const = 0;

    virtual void setWindowIndex(const int32_t windowIndex) = 0;

    virtual int32_t getGamepadIndex() const = 0;

    virtual void setGamepadIndex(const int32_t gamepadIndex) = 0;

    virtual float getForwardSpeed() const = 0;

    virtual void setForwardSpeed(const float forwardSpeed) = 0;

    virtual float getStrafeSpeed() const = 0;

    virtual void setStrafeSpeed(const float strafeSpeed) = 0;

    virtual float getUpSpeed() const = 0;

    virtual void setUpSpeed(const float upSpeed) = 0;

    virtual float getMoveMulitply() const = 0;

    virtual void setMoveMulitply(const float moveMulitply) = 0;

    virtual float getRotateMulitply() const = 0;

    virtual void setRotateMulitply(const float rotateMulitply) = 0;

    virtual float getYawSpeed() const = 0;

    virtual void setYawSpeed(const float yawSpeed) = 0;

    virtual float getPitchSpeed() const = 0;

    virtual void setPitchSpeed(const float pitchSpeed) = 0;

    virtual float getRollSpeed() const = 0;

    virtual void setRollSpeed(const float rollSpeed) = 0;

    virtual VkBool32 isPitchInverted() const = 0;

    virtual void setPitchInverted(const VkBool32 invert) = 0;

    virtual float getMouseMultiply() const = 0;

    virtual void setMouseMultiply(const float mouseMultiply) = 0;

    virtual const IMoveableSP& getMoveable() const = 0;

    virtual void setMoveable(const IMoveableSP& moveable) = 0;

};

typedef std::shared_ptr<IInputController> IInputControllerSP;

} /* namespace vkts */

#endif /* VKTS_IINPUTCONTROLLER_HPP_ */
