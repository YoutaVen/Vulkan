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

namespace vkts
{

void IMoveable::updateVectors()
{
    quat rotation = rotationZ * rotationY * rotationX;

    forward = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    left = rotation * glm::vec3(-1.0f, 0.0f, 0.0f);
    up = rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

IMoveable::IMoveable() :
    IUpdateable(), position(0.0f, 0.0f, 1.0f, 1.0f), rotationZ(), rotationY(), rotationX(), targetPosition(0.0f, 0.0f, 1.0f, 1.0f), targetRotationZ(), targetRotationY(), targetRotationX(), forward(0.0f, 0.0f, -1.0f), left(-1.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f)
{
}

IMoveable::IMoveable(const glm::vec4& position, const glm::vec3& rotation) :
    IUpdateable(), position(), rotationZ(), rotationY(), rotationX(), targetPosition(), targetRotationZ(), targetRotationY(), targetRotationX(), forward(0.0f, 0.0f, -1.0f), left(-1.0f, 0.0f, 0.0f), up(0.0f, 1.0f,0.0f)
{
    setPositionRotation(position, rotation);
}

IMoveable::IMoveable(const glm::vec4& position, const quat& rotationZ,
                     const quat& rotationY, const quat& rotationX) :
    IUpdateable(), position(), rotationZ(), rotationY(), rotationX(), targetPosition(), targetRotationZ(), targetRotationY(), targetRotationX(), forward(0.0f, 0.0f, -1.0f), left(-1.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f)
{
    setPositionRotation(position, rotationZ, rotationY, rotationX);
}

IMoveable::IMoveable(const IMoveable& other) :
    IUpdateable(), position(other.position), rotationZ(other.rotationZ), rotationY(other.rotationY), rotationX(other.rotationX), targetPosition(other.targetPosition), targetRotationZ(other.targetRotationZ), targetRotationY(other.targetRotationY), targetRotationX(other.targetRotationX), forward(other.forward), left(other.left), up(other.up)
{
}

IMoveable::~IMoveable()
{
}

IMoveable& IMoveable::operator =(const IMoveable& other)
{
    this->position = other.position;
    this->rotationZ = other.rotationZ;
    this->rotationY = other.rotationY;
    this->rotationX = other.rotationX;

    this->targetPosition = other.targetPosition;
    this->targetRotationZ = other.targetRotationZ;
    this->targetRotationY = other.targetRotationY;
    this->targetRotationX = other.targetRotationX;

    this->forward = other.forward;
    this->left = other.left;
    this->up = other.up;

    return *this;
}

//
// IMoveable
//

void IMoveable::setPositionRotation(const glm::vec4& position, const glm::vec3& rotation)
{
    this->position = position;
    this->rotationZ = rotateRz(rotation.z);
    this->rotationY = rotateRy(rotation.y);
    this->rotationX = rotateRx(rotation.x);

    this->targetPosition = this->position;
    this->targetRotationZ = this->rotationZ;
    this->targetRotationY = this->rotationY;
    this->targetRotationX = this->rotationX;

    updateVectors();
    update();
}

void IMoveable::setPositionRotation(const glm::vec4& position, const quat& rotationZ, const quat& rotationY, const quat& rotationX)
{
    this->position = position;
    this->rotationZ = rotationZ;
    this->rotationY = rotationY;
    this->rotationX = rotationX;

    this->targetPosition = this->position;
    this->targetRotationZ = this->rotationZ;
    this->targetRotationY = this->rotationY;
    this->targetRotationX = this->rotationX;

    updateVectors();
    update();
}

void IMoveable::setPosition(const glm::vec4& position)
{
    this->position = position;

    this->targetPosition = this->position;

    update();
}

void IMoveable::setRotation(const glm::vec3& rotation)
{
    this->rotationZ = rotateRz(rotation.z);
    this->rotationY = rotateRy(rotation.y);
    this->rotationX = rotateRx(rotation.x);

    this->targetRotationZ = this->rotationZ;
    this->targetRotationY = this->rotationY;
    this->targetRotationX = this->rotationX;

    updateVectors();
    update();
}

void IMoveable::setRotation(const quat& rotationZ, const quat& rotationY, const quat& rotationX)
{
    this->rotationZ = rotationZ;
    this->rotationY = rotationY;
    this->rotationX = rotationX;

    this->targetRotationZ = this->rotationZ;
    this->targetRotationY = this->rotationY;
    this->targetRotationX = this->rotationX;

    updateVectors();
    update();
}

void IMoveable::movePositionRotation(const glm::vec4& position, const glm::vec3& rotation)
{
    this->targetPosition = position;
    this->targetRotationZ = targetRotationZ * rotateRz(rotation.z);
    this->targetRotationY = targetRotationY * rotateRy(rotation.y);
    this->targetRotationX = targetRotationX * rotateRx(rotation.x);
}

void IMoveable::movePositionRotation(const glm::vec4& position, const quat& rotationZ, const quat& rotationY, const quat& rotationX)
{
    this->targetPosition = position;
    this->targetRotationZ = targetRotationZ * rotationZ;
    this->targetRotationY = targetRotationY * rotationY;
    this->targetRotationX = targetRotationX * rotationX;
}

void IMoveable::move(const float forwardFactor, const float strafeFactor, const float upFactor, const glm::vec3& rotation)
{
    this->targetPosition += glm::vec4(forward, 0.0f) * forwardFactor + glm::vec4(left, 0.0f) * strafeFactor + glm::vec4(up, 0.0f) * upFactor;
    this->targetRotationZ = targetRotationZ * rotateRz(rotation.z);
    this->targetRotationY = targetRotationY * rotateRy(rotation.y);
    this->targetRotationX = targetRotationX * rotateRx(rotation.x);
}

void IMoveable::move(const float forwardFactor, const float strafeFactor,
                     const float upFactor, const quat& rotationZ, const quat& rotationY,
                     const quat& rotationX)
{
    this->targetPosition += glm::vec4(forward, 0.0f) * forwardFactor + glm::vec4(left, 0.0f) * strafeFactor + glm::vec4(up, 0.0f) * upFactor;
    this->targetRotationZ = targetRotationZ * rotationZ;
    this->targetRotationY = targetRotationY * rotationY;
    this->targetRotationX = targetRotationX * rotationX;
}

//
// IUpdateable
//

VkBool32 IMoveable::update(const double deltaTime, const uint64_t deltaTicks)
{
    if (targetPosition != position || targetRotationZ != rotationZ || targetRotationY != rotationY || targetRotationX != rotationX)
    {
        glm::vec4 currentPosition = targetPosition + position;
        currentPosition /= currentPosition.w;

        quat currentRotationZ = slerp(targetRotationZ, rotationZ, 0.5f);
        quat currentRotationY = slerp(targetRotationY, rotationY, 0.5f);
        quat currentRotationX = slerp(targetRotationX, rotationX, 0.5f);

        this->position = currentPosition;
        this->rotationZ = currentRotationZ;
        this->rotationY = currentRotationY;
        this->rotationX = currentRotationX;

        updateVectors();
        update();
    }

    return VK_TRUE;
}

} /* namespace vkts */
