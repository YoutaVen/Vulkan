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

#ifndef VKTS_INPUTCONTROLLER_HPP_
#define VKTS_INPUTCONTROLLER_HPP_

#include <vkts/vkts.hpp>

#define VKTS_MOVE_MULTIPLY 4.0f
#define VKTS_ROTATE_MULTIPLY 2.0f

#define VKTS_FORWARD_SPEED 10.0f
#define VKTS_STRAFE_SPEED 10.0f
#define VKTS_UP_SPEED 10.0f

#define VKTS_YAW_SPEED 45.0f
#define VKTS_PITCH_SPEED 45.0f
#define VKTS_ROLL_SPEED 45.0f

#define VKTS_PITCH_MULTIPLY 1.0f

#define VKTS_MOUSE_MULTIPLY 1000.0f

namespace vkts
{

class InputController: public IInputController
{

private:

    const IUpdateThreadContext& updateThreadContext;

    int32_t windowIndex;
    int32_t gamepadIndex;

    glm::ivec2 lastMouseLocation;
    VkBool32 mouseLocationInitialized;

    float forwardSpeed;
    float strafeSpeed;
    float upSpeed;

    float moveMulitply;
    float rotateMulitply;

    float yawSpeed;
    float pitchSpeed;
    float rollSpeed;

    float pitchMulitply;

    float mouseMultiply;

    IMoveableSP moveable;

public:

    InputController() = delete;
    InputController(const IUpdateThreadContext& updateThreadContext, const int32_t windowIndex, const int32_t gamepadIndex);
    InputController(const IUpdateThreadContext& updateThreadContext, const int32_t windowIndex, const int32_t gamepadIndex, const IMoveableSP& moveable);
    InputController(const InputController& other);
    InputController(InputController&& other) = delete;
    virtual ~InputController();

    InputController& operator =(const InputController& other) = delete;
    InputController& operator =(InputController && other) = delete;

    //
    // IInputController
    //

    virtual int32_t getWindowIndex() const override;

    virtual void setWindowIndex(const int32_t windowIndex) override;

    virtual int32_t getGamepadIndex() const override;

    virtual void setGamepadIndex(const int32_t gamepadIndex) override;

    virtual float getForwardSpeed() const override;

    virtual void setForwardSpeed(const float forwardSpeed) override;

    virtual float getStrafeSpeed() const override;

    virtual void setStrafeSpeed(const float strafeSpeed) override;

    virtual float getUpSpeed() const override;

    virtual void setUpSpeed(const float upSpeed) override;

    virtual float getMoveMulitply() const override;

    virtual void setMoveMulitply(const float moveMulitply) override;

    virtual float getRotateMulitply() const override;

    virtual void setRotateMulitply(const float rotateMulitply) override;

    virtual float getYawSpeed() const override;

    virtual void setYawSpeed(const float yawSpeed) override;

    virtual float getPitchSpeed() const override;

    virtual void setPitchSpeed(const float pitchSpeed) override;

    virtual float getRollSpeed() const override;

    virtual void setRollSpeed(const float rollSpeed) override;

    virtual VkBool32 isPitchInverted() const override;

    virtual void setPitchInverted(const VkBool32 invert) override;

    virtual float getMouseMultiply() const override;

    virtual void setMouseMultiply(const float mouseMultiply) override;

    virtual const IMoveableSP& getMoveable() const override;

    virtual void setMoveable(const IMoveableSP& moveable) override;

    //
    // IUpdateable
    //

    virtual VkBool32 update(const double deltaTime, const uint64_t deltaTicks) override;

    //
    // ICloneable
    //

    virtual IInputControllerSP clone() const override;
};

} /* namespace vkts */

#endif /* VKTS_INPUTCONTROLLER_HPP_ */
