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

void plane::normalize()
{
	float length = glm::length(p);

	if (length != 0.0f)
	{
		p /= length;

		d /= length;
	}
}

plane::plane() :
	p(0.0f, 1.0f, 0.0f), d(0.0f)
{
}

plane::plane(const float x, const float y, const float z, const float d) :
	p(x, y, z), d(d)
{
	normalize();
}

plane::plane(const glm::vec3& normal, const float d) :
	p(normal), d(d)
{
	normalize();
}

plane::plane(const glm::vec3& normal, const glm::vec4& point) :
	p(glm::normalize(normal)), d(-glm::dot(glm::normalize(normal), glm::vec3(point)))
{
}

plane::~plane()
{
}

const glm::vec3& plane::getNormal() const
{
	return p;
}

float plane::getD() const
{
	return d;
}

float plane::distance(const glm::vec4& point) const
{
	return glm::dot(getNormal(), glm::vec3(point)) + getD();
}

float plane::distance(const sphere& sphere) const
{
	return glm::abs(distance(sphere.getCenter())) - sphere.getRadius();
}

float plane::distance(const plane& plane) const
{
	float dot = glm::dot(getNormal(), plane.getNormal());

	if (glm::abs(dot) < 1.0f)
	{
		return 0.0f;
	}

	if (dot > 0.0f)
	{
		return glm::abs(getD() - plane.getD());
	}

	return glm::abs(getD() + plane.getD());
}

//

plane operator* (const glm::mat4& matrix, const plane& p)
{
	glm::vec4 temp = matrix * glm::vec4(p.getNormal(), p.getD());

	return plane(glm::vec3(temp), temp.w);
}

} /* namespace vkts */
