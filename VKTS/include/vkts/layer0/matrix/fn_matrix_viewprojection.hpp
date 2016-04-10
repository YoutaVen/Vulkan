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

#ifndef VKTS_FN_MATRIX_VIEWPROJECTION_HPP_
#define VKTS_FN_MATRIX_VIEWPROJECTION_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY orthoMat4(const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY frustumMat4(const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY perspectiveMat4(const float fovy, const float aspect, const float zNear, const float zFar);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY lookAtMat4(const float eyeX, const float eyeY, const float eyeZ, const float centerX, const float centerY, const float centerZ, const float upX, const float upY, const float upZ);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY lookAtMat4(const glm::vec4& eye, const glm::vec4& center, const glm::vec3& up);

}

#endif /* VKTS_FN_MATRIX_VIEWPROJECTION_HPP_ */
