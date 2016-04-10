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

#ifndef VKTS_AABB_HPP_
#define VKTS_AABB_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class aabb
{

private:

	static const glm::vec4 cornersUnit[2];

	glm::vec4 corners[2];

	obb asObb;

	sphere asSphere;

	void calculateObb();

	void calculateSphere();

public:

    aabb();
    aabb(const glm::vec3& translate, const glm::vec3& scale);
    aabb(const glm::vec4& min, const glm::vec4& max);
    aabb(const float* vertices, const uint32_t numberVertices, const uint32_t stride);

    ~aabb();

    const glm::vec4& getCorner(const uint32_t i) const;

    const obb& getObb() const;

    const sphere& getSphere() const;

    VkBool32 intersect(const aabb& other) const;

    aabb operator +(const aabb& other) const;

    aabb& operator +=(const aabb& other);

};

} /* namespace vkts */

#endif /* VKTS_AABB_HPP_ */
