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

sphere::sphere() :
	c(0.0f, 0.0f, 0.0f, 1.0f), r(1.0f)
{
}

sphere::sphere(const float x, const float y, const float z, const float radius) :
	c(x, y, z, 1.0f), r(glm::abs(radius))
{
}

sphere::sphere(const glm::vec4& center, const float radius) :
	c(center), r(glm::abs(radius))
{
}

sphere::~sphere()
{
}

const glm::vec4& sphere::getCenter() const
{
	return c;
}

float sphere::getRadius() const
{
	return r;
}

float sphere::distance(const glm::vec4& point) const
{
	return glm::abs(glm::distance(c, point)) - r;
}

float sphere::distance(const plane& plane) const
{
	return plane.distance(*this);
}

float sphere::distance(const sphere& sphere) const
{
	return glm::abs(glm::distance(c, sphere.getCenter())) - r - sphere.getRadius();
}

sphere sphere::operator +(const sphere& other) const
{
	glm::vec4 center = (c + other.getCenter()) * 0.5f;

	float radius = glm::distance(c, center) + glm::max(r, other.getRadius());

	return sphere(center, radius);
}

sphere& sphere::operator +=(const sphere& other)
{
	glm::vec4 center = (c + other.getCenter()) * 0.5f;

	float radius = glm::distance(c, center) + glm::max(r, other.getRadius());

	//

	c = center;

	r = radius;

	return *this;
}

//

sphere operator *(const glm::mat4& transform, const sphere& s)
{
	float sx = glm::length(glm::vec3(glm::column(transform, 0)));
	float sy = glm::length(glm::vec3(glm::column(transform, 1)));
	float sz = glm::length(glm::vec3(glm::column(transform, 2)));

	float scale = glm::max(sx, glm::max(sy, sz));

	return sphere(transform * s.getCenter(), s.getRadius() * scale);
}

} /* namespace vkts */
