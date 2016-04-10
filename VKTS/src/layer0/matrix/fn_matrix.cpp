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

glm::mat4 VKTS_APIENTRY translateMat4(const float x, const float y, const float z)
{
    glm::mat4 matrix(1.0f);

    matrix[3][0] = x;
    matrix[3][1] = y;
    matrix[3][2] = z;

    return matrix;
}

glm::mat3 VKTS_APIENTRY translateMat3(const float x, const float y)
{
    glm::mat3 matrix(1.0f);

    matrix[2][0] = x;
    matrix[2][1] = y;

    return matrix;
}

glm::mat4 VKTS_APIENTRY rotateAxisMat4(const float angle, const float x, const float y, const float z)
{
    glm::mat4 matrix(1.0f);

    float s = sinf(glm::radians(angle));
    float c = cosf(glm::radians(angle));

    glm::vec3 v(x, y, z);

    v = glm::normalize(v);

    matrix[0][0] = v.x * v.x * (1 - c) + c;
    matrix[0][1] = v.x * v.y * (1 - c) + v.z * s;
    matrix[0][2] = v.x * v.z * (1 - c) - v.y * s;

    matrix[1][0] = v.x * v.y * (1 - c) - v.z * s;
    matrix[1][1] = v.y * v.y * (1 - c) + c;
    matrix[1][2] = v.y * v.z * (1 - c) + v.x * s;

    matrix[2][0] = v.x * v.z * (1 - c) + v.y * s;
    matrix[2][1] = v.y * v.z * (1 - c) - v.x * s;
    matrix[2][2] = v.z * v.z * (1 - c) + c;

    return matrix;
}

glm::mat3 VKTS_APIENTRY rotateAxisMat3(const float angle, const float x, const float y, const float z)
{
    glm::mat3 matrix(1.0f);

    float s = sinf(glm::radians(angle));
    float c = cosf(glm::radians(angle));

    glm::vec3 v(x, y, z);

    v = glm::normalize(v);

    matrix[0][0] = v.x * v.x * (1 - c) + c;
    matrix[0][1] = v.x * v.y * (1 - c) + v.z * s;
    matrix[0][2] = v.x * v.z * (1 - c) - v.y * s;
    matrix[1][0] = v.x * v.y * (1 - c) - v.z * s;
    matrix[1][1] = v.y * v.y * (1 - c) + c;
    matrix[1][2] = v.y * v.z * (1 - c) + v.x * s;
    matrix[2][0] = v.x * v.z * (1 - c) + v.y * s;
    matrix[2][1] = v.y * v.z * (1 - c) - v.x * s;
    matrix[2][2] = v.z * v.z * (1 - c) + c;

    return matrix;
}

glm::mat2 VKTS_APIENTRY rotateAxisMat2(const float angle)
{
    glm::mat2 matrix(1.0f);

    float s = sinf(glm::radians(angle));
    float c = cosf(glm::radians(angle));

    matrix[0][0] = c;
    matrix[0][1] = s;

    matrix[1][0] = -s;
    matrix[1][1] = c;

    return matrix;
}

glm::mat4 VKTS_APIENTRY rotateRxMat4(const float angle)
{
    glm::mat4 matrix(1.0f);

    float s = sinf(glm::radians(angle));
    float c = cosf(glm::radians(angle));

    matrix[1][1] = c;
    matrix[1][2] = s;

    matrix[2][1] = -s;
    matrix[2][2] = c;

    return matrix;
}

glm::mat3 VKTS_APIENTRY rotateRxMat3(const float angle)
{
    glm::mat3 matrix(1.0f);

    float s = sinf(glm::radians(angle));
    float c = cosf(glm::radians(angle));

    matrix[1][1] = c;
    matrix[1][2] = s;

    matrix[2][1] = -s;
    matrix[2][2] = c;

    return matrix;
}

glm::mat4 VKTS_APIENTRY rotateRyMat4(const float angle)
{
    glm::mat4 matrix(1.0f);

    float s = sinf(glm::radians(angle));
    float c = cosf(glm::radians(angle));

    matrix[0][0] = c;
    matrix[0][2] = -s;

    matrix[2][0] = s;
    matrix[2][2] = c;

    return matrix;
}

glm::mat3 VKTS_APIENTRY rotateRyMat3(const float angle)
{
    glm::mat3 matrix(1.0f);

    float s = sinf(glm::radians(angle));
    float c = cosf(glm::radians(angle));

    matrix[0][0] = c;
    matrix[0][2] = -s;

    matrix[2][0] = s;
    matrix[2][2] = c;

    return matrix;
}

glm::mat4 VKTS_APIENTRY rotateRzMat4(const float angle)
{
    glm::mat4 matrix(1.0f);

    float s = sinf(glm::radians(angle));
    float c = cosf(glm::radians(angle));

    matrix[0][0] = c;
    matrix[0][1] = s;

    matrix[1][0] = -s;
    matrix[1][1] = c;

    return matrix;
}

glm::mat3 VKTS_APIENTRY rotateRzMat3(const float angle)
{
    glm::mat3 matrix(1.0f);

    float s = sinf(glm::radians(angle));
    float c = cosf(glm::radians(angle));

    matrix[0][0] = c;
    matrix[0][1] = s;

    matrix[1][0] = -s;
    matrix[1][1] = c;

    return matrix;
}

glm::mat4 VKTS_APIENTRY rotateRzRxRyMat4(const float anglez, const float anglex, const float angley)
{
    glm::mat4 matrix(1.0f);

    float rx = glm::radians(anglex);
    float ry = glm::radians(angley);
    float rz = glm::radians(anglez);

    float sx = sinf(rx);
    float cx = cosf(rx);
    float sy = sinf(ry);
    float cy = cosf(ry);
    float sz = sinf(rz);
    float cz = cosf(rz);

    matrix[0][0] = cy * cz - sx * sy * sz;
    matrix[0][1] = cz * sx * sy + cy * sz;
    matrix[0][2] = -cx * sy;

    matrix[1][0] = -cx * sz;
    matrix[1][1] = cx * cz;
    matrix[1][2] = sx;

    matrix[2][0] = cz * sy + cy * sx * sz;
    matrix[2][1] = -cy * cz * sx + sy * sz;
    matrix[2][2] = cx * cy;

    return matrix;
}

glm::mat4 VKTS_APIENTRY rotateRzRyRxMat4(const float anglez, const float angley, const float anglex)
{
    glm::mat4 matrix(1.0f);

    float rx = glm::radians(anglex);
    float ry = glm::radians(angley);
    float rz = glm::radians(anglez);

    float sx = sinf(rx);
    float cx = cosf(rx);
    float sy = sinf(ry);
    float cy = cosf(ry);
    float sz = sinf(rz);
    float cz = cosf(rz);

    matrix[0][0] = cy * cz;
    matrix[0][1] = cy * sz;
    matrix[0][2] = -sy;

    matrix[1][0] = -cx * sz + cz * sx * sy;
    matrix[1][1] = cx * cz + sx * sy * sz;
    matrix[1][2] = cy * sx;

    matrix[2][0] = sz * sx + cx * cz * sy;
    matrix[2][1] = -cz * sx + cx * sy * sz;
    matrix[2][2] = cx * cy;

    return matrix;
}

glm::mat3 VKTS_APIENTRY rotateRzRxRyMat3(const float anglez, const float anglex, const float angley)
{
    glm::mat3 matrix(1.0f);

    float rx = glm::radians(anglex);
    float ry = glm::radians(angley);
    float rz = glm::radians(anglez);

    float sx = sinf(rx);
    float cx = cosf(rx);
    float sy = sinf(ry);
    float cy = cosf(ry);
    float sz = sinf(rz);
    float cz = cosf(rz);

    matrix[0][0] = cy * cz - sx * sy * sz;
    matrix[0][1] = cz * sx * sy + cy * sz;
    matrix[0][2] = -cx * sy;

    matrix[1][0] = -cx * sz;
    matrix[1][1] = cx * cz;
    matrix[1][2] = sx;

    matrix[2][0] = cz * sy + cy * sx * sz;
    matrix[2][1] = -cy * cz * sx + sy * sz;
    matrix[2][2] = cx * cy;

    return matrix;
}

glm::mat3 VKTS_APIENTRY rotateRzRyRxMat3(const float anglez, const float angley, const float anglex)
{
    glm::mat3 matrix(1.0f);

    float rx = glm::radians(anglex);
    float ry = glm::radians(angley);
    float rz = glm::radians(anglez);

    float sx = sinf(rx);
    float cx = cosf(rx);
    float sy = sinf(ry);
    float cy = cosf(ry);
    float sz = sinf(rz);
    float cz = cosf(rz);

    matrix[0][0] = cy * cz;
    matrix[0][1] = cy * sz;
    matrix[0][2] = -sy;

    matrix[1][0] = -cx * sz + cz * sx * sy;
    matrix[1][1] = cx * cz + sx * sy * sz;
    matrix[1][2] = cy * sx;

    matrix[2][0] = sz * sx + cx * cz * sy;
    matrix[2][1] = -cz * sx + cx * sy * sz;
    matrix[2][2] = cx * cy;

    return matrix;
}

glm::mat4 VKTS_APIENTRY scaleMat4(const float x, const float y, const float z)
{
    glm::mat4 matrix(1.0f);

    matrix[0][0] = x;
    matrix[1][1] = y;
    matrix[2][2] = z;

    return matrix;
}

glm::mat3 VKTS_APIENTRY scaleMat3(const float x, const float y, const float z)
{
    glm::mat3 matrix(1.0f);

    matrix[0][0] = x;
    matrix[1][1] = y;
    matrix[2][2] = z;

    return matrix;
}

glm::mat2 VKTS_APIENTRY scaleMat2(const float x, const float y)
{
    glm::mat2 matrix(1.0f);

    matrix[0][0] = x;
    matrix[1][1] = y;

    return matrix;
}

glm::mat4 VKTS_APIENTRY shearMat4(const float shxy, const float shxz, const float shyx, const float shyz, const float shzx, const float shzy)
{
    glm::mat4 matrix(1.0f);

    matrix[1][0] = shxy;
    matrix[2][0] = shxz;

    matrix[0][1] = shyx;
    matrix[2][1] = shyz;

    matrix[0][2] = shzx;
    matrix[1][2] = shzy;

    return matrix;
}

glm::mat3 VKTS_APIENTRY shearMat3(const float shxy, const float shxz, const float shyx, const float shyz, const float shzx, const float shzy)
{
    glm::mat3 matrix(1.0f);

    matrix[1][0] = shxy;
    matrix[2][0] = shxz;

    matrix[0][1] = shyx;
    matrix[2][1] = shyz;

    matrix[0][2] = shzx;
    matrix[1][2] = shzy;

    return matrix;
}

glm::mat2 VKTS_APIENTRY shearMat2(const float shx, const float shy)
{
    glm::mat2 matrix(1.0f);

    matrix[1][0] = shx;

    matrix[0][1] = shy;

    return matrix;
}

}
