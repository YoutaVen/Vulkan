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

#include "Camera.hpp"

namespace vkts
{

ICameraSP VKTS_APIENTRY cameraCreate(const glm::vec4& position, const glm::vec3& rotation)
{
    auto newInstance = new Camera(position, rotation);

    if (!newInstance)
    {
        return ICameraSP();
    }

    return ICameraSP(newInstance);
}

ICameraSP VKTS_APIENTRY cameraCreate(const glm::vec4& position, const quat& rotationZ, const quat& rotationY, const quat& rotationX)
{
    auto newInstance = new Camera(position, rotationZ, rotationY, rotationX);

    if (!newInstance)
    {
        return ICameraSP();
    }

    return ICameraSP(newInstance);
}

ICameraSP VKTS_APIENTRY cameraCreate(const glm::vec4& position, const glm::vec4& center)
{
	glm::vec3 rotation(0.0f, 0.0f, 0.0f);

	//

    auto forward = glm::normalize(glm::vec3(center - position));

    auto forwardXZ = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));

    //

    rotation.x = 90.0f - glm::degrees(acosf(glm::dot(forward, glm::vec3(0.0f, 1.0f, 0.0f))));

    rotation.y = glm::degrees(acosf(glm::dot(forwardXZ, glm::vec3(0.0f, 0.0f, -1.0f))));
    if (glm::dot(forwardXZ, glm::vec3(-1.0f, 0.0f, 0.0f)) < 0.0f)
	{
    	rotation.y = -rotation.y;
	}

    rotation.z = 0.0f;

    //

    auto newInstance = new Camera(position, rotation);

    if (!newInstance)
    {
        return ICameraSP();
    }

    return ICameraSP(newInstance);
}

}
