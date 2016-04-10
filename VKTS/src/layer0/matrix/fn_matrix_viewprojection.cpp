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

// Deriving projection matrix:
// http://www.songho.ca/opengl/gl_projectionmatrix.html
// http://www.codeguru.com/cpp/misc/misc/graphics/article.php/c10123/Deriving-Projection-Matrices.htm#page-1

glm::mat4 VKTS_APIENTRY orthoMat4(const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal)
{
    if ((right - left) == 0.0f || (top - bottom) == 0.0f || (farVal - nearVal) == 0.0f)
    {
        return glm::mat4();
    }

    glm::mat4 result;

    result[0][0] = 2.0f / (right - left);
    result[0][1] = 0.0f;
    result[0][2] = 0.0f;
    result[0][3] = 0.0f;
    result[1][0] = 0.0f;
    // Window clip origin is upper left.
    result[1][1] = -2.0f / (top - bottom);
    result[1][2] = 0.0f;
    result[1][3] = 0.0f;
    result[2][0] = 0.0f;
    result[2][1] = 0.0f;
    // z range is [0 1]
    result[2][2] = -1.0f / (farVal - nearVal);
    result[2][3] = 0.0f;
    result[3][0] = -(right + left) / (right - left);
    result[3][1] = -(top + bottom) / (top - bottom);
    // z range is [0 1]
    result[3][2] = -nearVal / (farVal - nearVal);
    result[3][3] = 1.0f;

    return result;
}

glm::mat4 VKTS_APIENTRY frustumMat4(const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal)
{
    if ((right - left) == 0.0f || (top - bottom) == 0.0f || (farVal - nearVal) == 0.0f)
    {
        return glm::mat4();
    }

    glm::mat4 result;

    result[0][0] = 2.0f * nearVal / (right - left);
    result[0][1] = 0.0f;
    result[0][2] = 0.0f;
    result[0][3] = 0.0f;
    result[1][0] = 0.0f;
    // Window clip origin is upper left.
    result[1][1] = -2.0f * nearVal / (top - bottom);
    result[1][2] = 0.0f;
    result[1][3] = 0.0f;
    result[2][0] = (right + left) / (right - left);
    result[2][1] = (top + bottom) / (top - bottom);
    // z range is [0 1]
    result[2][2] = -farVal / (farVal - nearVal);
    result[2][3] = -1.0f;
    result[3][0] = 0.0f;
    result[3][1] = 0.0f;
    // z range is [0 1]
    result[3][2] = -(farVal * nearVal) / (farVal - nearVal);
    result[3][3] = 0.0f;

    return result;
}

glm::mat4 VKTS_APIENTRY perspectiveMat4(const float fovy, const float aspect, const float zNear, const float zFar)
{
    if (fovy <= 0.0f || fovy >= 180.0f)
    {
        return glm::mat4();
    }

    float xmin, xmax, ymin, ymax;

    ymax = zNear * tanf(glm::radians(fovy * 0.5f));
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    return frustumMat4(xmin, xmax, ymin, ymax, zNear, zFar);
}

glm::mat4 VKTS_APIENTRY lookAtMat4(const float eyeX, const float eyeY, const float eyeZ, const float centerX, const float centerY, const float centerZ, const float upX, const float upY, const float upZ)
{
    glm::vec3 forward;
    glm::vec3 side;
    glm::vec3 up;

    forward[0] = centerX - eyeX;
    forward[1] = centerY - eyeY;
    forward[2] = centerZ - eyeZ;

    forward = glm::normalize(forward);

    up[0] = upX;
    up[1] = upY;
    up[2] = upZ;

    side = glm::cross(forward, up);
    side = glm::normalize(side);

    up = glm::cross(side, forward);

    glm::mat4 result;

    result[0][0] = side[0];
    result[0][1] = up[0];
    result[0][2] = -forward[0];
    result[0][3] = 0.0f;
    result[1][0] = side[1];
    result[1][1] = up[1];
    result[1][2] = -forward[1];
    result[1][3] = 0.0f;
    result[2][0] = side[2];
    result[2][1] = up[2];
    result[2][2] = -forward[2];
    result[2][3] = 0.0f;
    result[3][0] = 0.0f;
    result[3][1] = 0.0f;
    result[3][2] = 0.0f;
    result[3][3] = 1.0f;

    return result * translateMat4(-eyeX, -eyeY, -eyeZ);
}

glm::mat4 VKTS_APIENTRY lookAtMat4(const glm::vec4& eye, const glm::vec4& center, const glm::vec3& up)
{
    return lookAtMat4(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
}

}
