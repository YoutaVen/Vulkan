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

const glm::vec4 aabb::cornersUnit[2] = {glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)};

void aabb::calculateObb()
{
	glm::vec4 center = (corners[0] + corners[1]) * 0.5f;

	asObb = obb(glm::vec3(center), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(corners[1] - center));
}

void aabb::calculateSphere()
{
	glm::vec4 center = (corners[0] + corners[1]) * 0.5f;

	float radius = glm::distance(corners[1], center);

	asSphere = sphere(center, radius);
}

aabb::aabb() :
	aabb(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
{
}

aabb::aabb(const glm::vec3& translate, const glm::vec3& scale) :
	corners(), asObb(), asSphere()
{
	glm::mat4 transfom = translateMat4(translate.x, translate.y, translate.z) * scaleMat4(scale.x, scale.y, scale.z);

	for (int32_t i = 0; i < 2; i++)
	{
		corners[i] = transfom * cornersUnit[i];
	}

	calculateObb();

	calculateSphere();
}

aabb::aabb(const glm::vec4& min, const glm::vec4& max) :
	corners{min, max}, asObb(), asSphere()
{
	calculateObb();

	calculateSphere();
}

aabb::aabb(const float* vertices, const uint32_t numberVertices, const uint32_t stride) :
	corners(), asObb(), asSphere()
{
	glm::vec4 min;

	glm::vec4 max;

	if (vertices)
	{
		const uint8_t* p = (const uint8_t*)vertices;

		for (uint32_t i = 0; i < numberVertices; i++)
		{
			const float* v = (const float*)(&p[i * stride]);

			glm::vec4 currentVertex = glm::vec4(v[0], v[1], v[2], v[3]);

			if (i == 0)
			{
				min = currentVertex;
				max = currentVertex;
			}
			else
			{
				for (int32_t element = 0; element < 3; element++)
				{
					if (currentVertex[element] < min[element])
					{
						min[element] = currentVertex[element];
					}
					else if (currentVertex[element] > max[element])
					{
						max[element] = currentVertex[element];
					}
				}
			}
		}
	}
	else
	{
		min = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		max = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	//

	corners[0] = min;
	corners[1] = max;

	//

	calculateObb();

	calculateSphere();
}

aabb::~aabb()
{
}

const glm::vec4& aabb::getCorner(const uint32_t i) const
{
	// No check by purpose.
	return corners[i];
}

const obb& aabb::getObb() const
{
	return asObb;
}

const sphere& aabb::getSphere() const
{
	return asSphere;
}

VkBool32 aabb::intersect(const aabb& other) const
{
	for (int32_t element = 0; element < 3; element++)
	{
		if (corners[1][element] < other.corners[0][element] || corners[0][element] > other.corners[1][element])
		{
			return VK_FALSE;
		}
	}

	return VK_TRUE;
}

aabb aabb::operator +(const aabb& other) const
{
	aabb result(*this);

	for (int32_t element = 0; element < 3; element++)
	{
		if (other.corners[0][element] < result.corners[0][element])
		{
			result.corners[0][element] = other.corners[0][element];
		}

		if (other.corners[1][element] > result.corners[1][element])
		{
			result.corners[1][element] = other.corners[1][element];
		}
	}

	//

	result.calculateObb();

	result.calculateSphere();

	//

	return result;
}

aabb& aabb::operator +=(const aabb& other)
{
	for (int32_t element = 0; element < 3; element++)
	{
		if (other.corners[0][element] < corners[0][element])
		{
			corners[0][element] = other.corners[0][element];
		}

		if (other.corners[1][element] > corners[1][element])
		{
			corners[1][element] = other.corners[1][element];
		}
	}

	//

	calculateObb();

	calculateSphere();

	//

	return *this;
}

} /* namespace vkts */
