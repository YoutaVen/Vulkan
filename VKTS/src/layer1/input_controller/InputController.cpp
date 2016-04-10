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

#include "InputController.hpp"

namespace vkts
{

InputController::InputController(const IUpdateThreadContext& updateThreadContext, const int32_t windowIndex, const int32_t gamepadIndex) :
    IInputController(), updateThreadContext(updateThreadContext), windowIndex(windowIndex), gamepadIndex(gamepadIndex), lastMouseLocation(0, 0), mouseLocationInitialized(VK_FALSE), forwardSpeed(VKTS_FORWARD_SPEED), strafeSpeed(VKTS_STRAFE_SPEED), upSpeed(VKTS_UP_SPEED), moveMulitply(VKTS_MOVE_MULTIPLY), rotateMulitply(VKTS_ROTATE_MULTIPLY), yawSpeed(VKTS_YAW_SPEED), pitchSpeed(VKTS_PITCH_SPEED), rollSpeed(VKTS_ROLL_SPEED), pitchMulitply(VKTS_PITCH_MULTIPLY), mouseMultiply(VKTS_MOUSE_MULTIPLY), moveable()
{
}

InputController::InputController(const IUpdateThreadContext& updateThreadContext, const int32_t windowIndex, const int32_t gamepadIndex, const IMoveableSP& moveable) :
    IInputController(), updateThreadContext(updateThreadContext), windowIndex(windowIndex), gamepadIndex(gamepadIndex), lastMouseLocation(0,0), mouseLocationInitialized(VK_FALSE), forwardSpeed(VKTS_FORWARD_SPEED), strafeSpeed(VKTS_STRAFE_SPEED), upSpeed(VKTS_UP_SPEED), moveMulitply(VKTS_MOVE_MULTIPLY), rotateMulitply(VKTS_ROTATE_MULTIPLY), yawSpeed(VKTS_YAW_SPEED), pitchSpeed(VKTS_PITCH_SPEED), rollSpeed(VKTS_ROLL_SPEED), pitchMulitply(VKTS_PITCH_MULTIPLY), mouseMultiply(VKTS_MOUSE_MULTIPLY), moveable(moveable)
{
}

InputController::InputController(const InputController& other) :
    IInputController(), updateThreadContext(other.updateThreadContext), windowIndex(other.windowIndex), gamepadIndex(other.gamepadIndex), lastMouseLocation(other.lastMouseLocation), mouseLocationInitialized(other.mouseLocationInitialized), forwardSpeed(other.forwardSpeed), strafeSpeed(other.strafeSpeed), upSpeed(other.upSpeed), moveMulitply(other.moveMulitply), rotateMulitply(other.rotateMulitply), yawSpeed(other.yawSpeed), pitchSpeed(other.pitchSpeed), rollSpeed(other.rollSpeed), pitchMulitply(other.pitchMulitply), mouseMultiply(other.mouseMultiply), moveable(other.moveable)
{
}

InputController::~InputController()
{
}

//
// IInputController
//

int32_t InputController::getWindowIndex() const
{
    return windowIndex;
}

void InputController::setWindowIndex(const int32_t windowIndex)
{
    this->windowIndex = windowIndex;
}

int32_t InputController::getGamepadIndex() const
{
    return gamepadIndex;
}

void InputController::setGamepadIndex(const int32_t gamepadIndex)
{
    this->gamepadIndex = gamepadIndex;
}

float InputController::getForwardSpeed() const
{
    return forwardSpeed;
}

void InputController::setForwardSpeed(const float forwardSpeed)
{
    this->forwardSpeed = forwardSpeed;
}

float InputController::getStrafeSpeed() const
{
    return strafeSpeed;
}

void InputController::setStrafeSpeed(const float strafeSpeed)
{
    this->strafeSpeed = strafeSpeed;
}

float InputController::getUpSpeed() const
{
    return upSpeed;
}

void InputController::setUpSpeed(const float upSpeed)
{
    this->upSpeed = upSpeed;
}

float InputController::getMoveMulitply() const
{
    return moveMulitply;
}

void InputController::setMoveMulitply(const float moveMulitply)
{
    this->moveMulitply = moveMulitply;
}

float InputController::getRotateMulitply() const
{
    return rotateMulitply;
}

void InputController::setRotateMulitply(const float rotateMulitply)
{
    this->rotateMulitply = rotateMulitply;
}

float InputController::getYawSpeed() const
{
    return yawSpeed;
}

void InputController::setYawSpeed(const float yawSpeed)
{
    this->yawSpeed = yawSpeed;
}

float InputController::getPitchSpeed() const
{
    return pitchSpeed;
}

void InputController::setPitchSpeed(const float pitchSpeed)
{
    this->pitchSpeed = pitchSpeed;
}

float InputController::getRollSpeed() const
{
    return rollSpeed;
}

void InputController::setRollSpeed(const float rollSpeed)
{
    this->rollSpeed = rollSpeed;
}

VkBool32 InputController::isPitchInverted() const
{
    return pitchMulitply < 0.0f;
}

void InputController::setPitchInverted(const VkBool32 invert)
{
    this->pitchMulitply = invert ? -1.0f : 1.0f;
}

float InputController::getMouseMultiply() const
{
    return mouseMultiply;
}

void InputController::setMouseMultiply(const float mouseMultiply)
{
    this->mouseMultiply = mouseMultiply;
}

const IMoveableSP& InputController::getMoveable() const
{
    return moveable;
}

void InputController::setMoveable(const IMoveableSP& moveable)
{
    this->moveable = moveable;
}

//
// IUpdateable
//

VkBool32 InputController::update(const double deltaTime, const uint64_t deltaTicks)
{
    if (moveable.get())
    {
        if (windowIndex >= 0)
        {
            if (gamepadIndex >= 0 && gamepadIndex < VKTS_MAX_GAMEPADS)
            {
                // Gamepad

                float moveSpeedFactor = 1.0f;
                float rotateSpeedFactor = 1.0f;
                if (updateThreadContext.getGamepadButton(windowIndex, gamepadIndex, VKTS_GAMEPAD_LEFT_THUMB))
                {
                    moveSpeedFactor = moveMulitply;
                    rotateSpeedFactor = rotateMulitply;
                }

                float forwardFactor = updateThreadContext.getGamepadAxis(windowIndex, gamepadIndex, VKTS_GAMEPAD_LEFT_STICK_Y) * forwardSpeed * moveSpeedFactor * (float) deltaTime;
                float strafeFactor = -updateThreadContext.getGamepadAxis(windowIndex, gamepadIndex, VKTS_GAMEPAD_LEFT_STICK_X) * strafeSpeed * moveSpeedFactor * (float) deltaTime;
                float upFactor = 0.0f;

                glm::vec3 deltaRotation(0.0f, 0.0f, 0.0f);

                if (updateThreadContext.getGamepadButton(windowIndex, gamepadIndex, VKTS_GAMEPAD_RIGHT_THUMB))
                {
                    upFactor = updateThreadContext.getGamepadAxis(windowIndex, gamepadIndex, VKTS_GAMEPAD_RIGHT_STICK_Y) * upSpeed * moveSpeedFactor * pitchMulitply * (float) deltaTime;
                }
                else
                {
                    deltaRotation.x = updateThreadContext.getGamepadAxis(windowIndex, gamepadIndex, VKTS_GAMEPAD_RIGHT_STICK_Y) * pitchSpeed * rotateSpeedFactor * pitchMulitply * (float) deltaTime;
                }

                deltaRotation.y = -updateThreadContext.getGamepadAxis(windowIndex, gamepadIndex, VKTS_GAMEPAD_RIGHT_STICK_X) * yawSpeed * rotateSpeedFactor * (float) deltaTime;

                moveable->move(forwardFactor, strafeFactor, upFactor, deltaRotation);
            }
            else if (gamepadIndex < 0)
            {
                // Keyboard

                float moveSpeedFactor = 1.0f;
                float rotateSpeedFactor = 1.0f;
                if (updateThreadContext.getKey(windowIndex, VKTS_KEY_LEFT_SHIFT))
                {
                    moveSpeedFactor = moveMulitply;
                    rotateSpeedFactor = rotateMulitply;
                }

                float forwardFactor = 0.0f;
                if (updateThreadContext.getKey(windowIndex, VKTS_KEY_W))
                {
                    forwardFactor += 1.0f * forwardSpeed * moveSpeedFactor * (float) deltaTime;
                }
                if (updateThreadContext.getKey(windowIndex, VKTS_KEY_S))
                {
                    forwardFactor -= 1.0f * forwardSpeed * moveSpeedFactor * (float) deltaTime;
                }

                float strafeFactor = 0.0f;
                if (updateThreadContext.getKey(windowIndex, VKTS_KEY_D))
                {
                    strafeFactor -= 1.0f * strafeSpeed * moveSpeedFactor * (float) deltaTime;
                }
                if (updateThreadContext.getKey(windowIndex, VKTS_KEY_A))
                {
                    strafeFactor += 1.0f * strafeSpeed * moveSpeedFactor * (float) deltaTime;
                }

                float upFactor = 0.0f;
                if (updateThreadContext.getKey(windowIndex, VKTS_KEY_PAGE_UP))
                {
                    upFactor += 1.0f * upSpeed * moveSpeedFactor * (float) deltaTime;
                }
                if (updateThreadContext.getKey(windowIndex, VKTS_KEY_PAGE_DOWN))
                {
                    upFactor -= 1.0f * upSpeed * moveSpeedFactor * (float) deltaTime;
                }

                glm::vec3 deltaRotation(0.0f, 0.0f, 0.0f);

                const auto& currentMouseLocation = updateThreadContext.getMouseLocation(windowIndex);

                if (updateThreadContext.isGameMouse(windowIndex))
                {
                    if (!mouseLocationInitialized)
                    {
                        if (currentMouseLocation.y == (int32_t)updateThreadContext.getWindowDimension(windowIndex).y / 2 && currentMouseLocation.x == (int32_t)updateThreadContext.getWindowDimension(windowIndex).x / 2)
                        {
                            mouseLocationInitialized = VK_TRUE;
                        }
                    }
                    else
                    {
                        deltaRotation.x = -(float)(2 * (currentMouseLocation.y - (int32_t)updateThreadContext.getWindowDimension(windowIndex).y / 2)) / (float) updateThreadContext.getWindowDimension(windowIndex).y * pitchSpeed * rotateSpeedFactor * pitchMulitply * (float) deltaTime * mouseMultiply;
                        deltaRotation.y = -(float)(2 * (currentMouseLocation.x - (int32_t)updateThreadContext.getWindowDimension(windowIndex).x / 2)) / (float) updateThreadContext.getWindowDimension(windowIndex).x * yawSpeed * rotateSpeedFactor * (float) deltaTime * mouseMultiply;
                    }
                }
                else
                {
                    if (!mouseLocationInitialized)
                    {
                        lastMouseLocation = currentMouseLocation;

                        mouseLocationInitialized = VK_TRUE;
                    }

                    deltaRotation.x = -(float)(2 * (currentMouseLocation.y - lastMouseLocation.y)) / (float) updateThreadContext.getWindowDimension(windowIndex).y * pitchSpeed * rotateSpeedFactor * pitchMulitply * (float) deltaTime * mouseMultiply;
                    deltaRotation.y = -(float)(2 * (currentMouseLocation.x - lastMouseLocation.x)) / (float) updateThreadContext.getWindowDimension(windowIndex).x * yawSpeed * rotateSpeedFactor * (float) deltaTime * mouseMultiply;

                    lastMouseLocation = glm::ivec2(currentMouseLocation.x, currentMouseLocation.y);
                }

                moveable->move(forwardFactor, strafeFactor, upFactor, deltaRotation);
            }
        }
    }

    return VK_TRUE;
}

//
// ICloneable
//

IInputControllerSP InputController::clone() const
{
    return IInputControllerSP(new InputController(*this));
}

} /* namespace vkts */
