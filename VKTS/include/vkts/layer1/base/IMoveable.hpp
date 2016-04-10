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

#ifndef VKTS_IMOVEABLE_HPP_
#define VKTS_IMOVEABLE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class IMoveable: public IUpdateable
{

protected:

    glm::vec4 position;
    quat rotationZ;
    quat rotationY;
    quat rotationX;

    glm::vec4 targetPosition;
    quat targetRotationZ;
    quat targetRotationY;
    quat targetRotationX;

    glm::vec3 forward;
    glm::vec3 left;
    glm::vec3 up;

    void updateVectors();

    virtual void update() = 0;

public:

    IMoveable();
    IMoveable(const glm::vec4& position, const glm::vec3& rotation);
    IMoveable(const glm::vec4& position, const quat& rotationZ, const quat& rotationY, const quat& rotationX);
    IMoveable(const IMoveable& other);
    IMoveable(IMoveable&& other) = delete;
    virtual ~IMoveable();

    IMoveable& operator =(const IMoveable& other);
    IMoveable& operator =(IMoveable && other) = delete;

    void setPositionRotation(const glm::vec4& position, const glm::vec3& rotation);
    void setPositionRotation(const glm::vec4& position, const quat& rotationZ, const quat& rotationY, const quat& rotationX);

    void setPosition(const glm::vec4& position);

    void setRotation(const glm::vec3& rotation);

    void setRotation(const quat& rotationZ, const quat& rotationY, const quat& rotationX);

    //

    void movePositionRotation(const glm::vec4& position, const glm::vec3& rotation);

    void movePositionRotation(const glm::vec4& position, const quat& rotationZ, const quat& rotationY, const quat& rotationX);

    void move(const float forwardFactor, const float strafeFactor, const float upFactor, const glm::vec3& rotation);

    void move(const float forwardFactor, const float strafeFactor, const float upFactor, const quat& rotationZ, const quat& rotationY, const quat& rotationX);

    virtual VkBool32 update(const double deltaTime, const uint64_t deltaTicks) override;

};

typedef std::shared_ptr<IMoveable> IMoveableSP;

} /* namespace vkts */

#endif /* VKTS_IMOVEABLE_HPP_ */
