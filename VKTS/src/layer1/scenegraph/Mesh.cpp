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

#include "Mesh.hpp"

namespace vkts
{

Mesh::Mesh() :
    IMesh(), name(""), allSubMeshes(), displace(0.0f, 0.0f), box(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
{
}

Mesh::Mesh(const Mesh& other) :
    IMesh(), name(other.name), allSubMeshes(), displace(other.displace), box(other.box)
{
    for (size_t i = 0; i < other.allSubMeshes.size(); i++)
    {
        auto currentSubMesh = other.allSubMeshes[i]->clone();

        if (currentSubMesh.get())
        {
            allSubMeshes.append(currentSubMesh);
        }
    }
}

Mesh::~Mesh()
{
    destroy();
}

//
// IMesh
//

const std::string& Mesh::getName() const
{
    return name;
}

void Mesh::setName(const std::string& name)
{
    this->name = name;
}

void Mesh::addSubMesh(const ISubMeshSP& subMesh)
{
    allSubMeshes.append(subMesh);

    //

    if (allSubMeshes.size() == 1)
    {
    	this->box = subMesh->getAABB();
    }
    else
    {
    	this->box += subMesh->getAABB();
    }
}

VkBool32 Mesh::removeSubMesh(const ISubMeshSP& subMesh)
{
    return allSubMeshes.remove(subMesh);
}

size_t Mesh::getNumberSubMeshes() const
{
    return allSubMeshes.size();
}

const SmartPointerVector<ISubMeshSP>& Mesh::getSubMeshes() const
{
    return allSubMeshes;
}

void Mesh::updateDescriptorSetsRecursive(const std::string& nodeName, const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets)
{
    for (size_t i = 0; i < allSubMeshes.size(); i++)
    {
        allSubMeshes[i]->updateDescriptorSetsRecursive(nodeName, allWriteDescriptorSetsCount, allWriteDescriptorSets);
    }
}

void Mesh::bindDrawIndexedRecursive(const std::string& nodeName, const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const overwrite* renderOverwrite, const uint32_t bufferIndex) const
{
    const overwrite* currentOverwrite = renderOverwrite;
    while (currentOverwrite)
    {
    	if (!currentOverwrite->meshBindDrawIndexedRecursive(*this, cmdBuffer, allGraphicsPipelines, bufferIndex))
    	{
    		return;
    	}

    	currentOverwrite = currentOverwrite->getNextOverwrite();
    }

    for (size_t i = 0; i < allSubMeshes.size(); i++)
    {
        allSubMeshes[i]->bindDrawIndexedRecursive(nodeName, cmdBuffer, allGraphicsPipelines, renderOverwrite, bufferIndex);
    }
}


void Mesh::setDisplace(const glm::vec2& displace)
{
	this->displace = displace;
}

const glm::vec2& Mesh::getDisplace() const
{
	return displace;
}

void Mesh::setAABB(const aabb& box)
{
	this->box = box;
}

const aabb& Mesh::getAABB() const
{
	return box;
}

//
// ICloneable
//

IMeshSP Mesh::clone() const
{
    return IMeshSP(new Mesh(*this));
}

//
// IDestroyable
//

void Mesh::destroy()
{
    for (size_t i = 0; i < allSubMeshes.size(); i++)
    {
        allSubMeshes[i]->destroy();
    }
    allSubMeshes.clear();
}

} /* namespace vkts */
