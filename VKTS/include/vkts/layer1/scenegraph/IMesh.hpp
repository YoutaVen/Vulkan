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

#ifndef VKTS_IMESH_HPP_
#define VKTS_IMESH_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class overwrite;

class IMesh: public ICloneable<IMesh>, public IDestroyable
{

public:

    IMesh() :
        ICloneable<IMesh>(), IDestroyable()
    {
    }

    virtual ~IMesh()
    {
    }

    virtual const std::string& getName() const = 0;

    virtual void setName(const std::string& name) = 0;

    virtual void addSubMesh(const ISubMeshSP& subMesh) = 0;

    virtual VkBool32 removeSubMesh(const ISubMeshSP& subMesh) = 0;

    virtual size_t getNumberSubMeshes() const = 0;

    virtual const SmartPointerVector<ISubMeshSP>& getSubMeshes() const = 0;

    virtual void updateDescriptorSetsRecursive(const std::string& nodeName, const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets) = 0;

    virtual void bindDrawIndexedRecursive(const std::string& nodeName, const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const overwrite* renderOverwrite = nullptr, const uint32_t bufferIndex = 0) const = 0;

    virtual void setDisplace(const glm::vec2& displace) = 0;

    virtual const glm::vec2& getDisplace() const = 0;

    virtual void setAABB(const aabb& box) = 0;

    virtual const aabb& getAABB() const = 0;

};

typedef std::shared_ptr<IMesh> IMeshSP;

} /* namespace vkts */

#endif /* VKTS_IMESH_HPP_ */
