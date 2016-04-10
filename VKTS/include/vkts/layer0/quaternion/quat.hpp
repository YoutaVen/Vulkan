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

#ifndef VKTS_QUAT_HPP_
#define VKTS_QUAT_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class quat
{

private:

    void multiply(quat& result, const quat& q0, const quat& q1) const;

public:

    float x;
    float y;
    float z;
    float w;

    quat();
    quat(const float x, const float y, const float z, const float w);
    quat(const quat& other);
    quat(const glm::vec3& other);
    quat(const glm::vec4& other);
    ~quat();

    bool operator ==(const quat& other) const;

    bool operator !=(const quat& other) const;


    quat operator +(const quat& other) const;

    quat& operator +=(const quat& other);

    quat operator -(const quat& other) const;

    quat& operator -=(const quat& other);

    quat operator *(const quat& other) const;

    quat& operator *=(const quat& other);

    quat operator *(const float other) const;

    quat& operator *=(const float other);

    quat operator /(const float other) const;

    quat& operator /=(const float other);

    quat& operator =(const quat& other);


    glm::vec3 operator *(const glm::vec3& other) const;

    glm::vec4 operator *(const glm::vec4& other) const;

    glm::mat3 mat3() const;

    glm::mat4 mat4() const;

    glm::vec3 rotation() const;

};

} /* namespace vkts */

#endif /* VKTS_QUAT_HPP_ */
