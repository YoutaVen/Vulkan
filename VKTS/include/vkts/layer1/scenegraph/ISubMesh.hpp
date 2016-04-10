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

#ifndef VKTS_ISUBMESH_HPP_
#define VKTS_ISUBMESH_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class overwrite;

class ISubMesh: public IDestroyable, public ICloneable<ISubMesh>
{

public:

    ISubMesh() :
        IDestroyable()
    {
    }

    virtual ~ISubMesh()
    {
    }

    virtual const std::string& getName() const = 0;

    virtual void setName(const std::string& name) = 0;

    virtual const IBufferObjectSP& getVertexBuffer() const = 0;

    virtual VkTsVertexBufferType getVertexBufferType() const = 0;

    virtual void setVertexBuffer(const IBufferObjectSP& vertexBuffer, const VkTsVertexBufferType vertexBufferType, const aabb& verticesAABB) = 0;

    virtual int32_t getNumberVertices() const = 0;

    virtual void setNumberVertices(const int32_t numberVertices) = 0;

    virtual const IBufferObjectSP& getIndicesVertexBuffer() const = 0;

    virtual void setIndicesVertexBuffer(const IBufferObjectSP& indicesVertexBuffer) = 0;

    virtual int32_t getNumberIndices() const = 0;

    virtual void setNumberIndices(const int32_t numberIndices) = 0;

    virtual VkIndexType getIndexType() const = 0;

    virtual VkPrimitiveTopology getPrimitiveTopology() const = 0;

    virtual void setPrimitiveTopology(const VkPrimitiveTopology primitiveTopology) = 0;

    virtual const IBSDFMaterialSP& getBSDFMaterial() const = 0;

    virtual void setBSDFMaterial(const IBSDFMaterialSP& bsdfMaterial) = 0;

    virtual const IPhongMaterialSP& getPhongMaterial() const = 0;

    virtual void setPhongMaterial(const IPhongMaterialSP& phongMaterial) = 0;

    virtual int32_t getVertexOffset() const = 0;

    virtual void setVertexOffset(const int32_t vertexOffset) = 0;

    virtual int32_t getNormalOffset() const = 0;

    virtual void setNormalOffset(const int32_t normalOffset) = 0;

    virtual int32_t getBitangentOffset() const = 0;

    virtual void setBitangentOffset(const int32_t bitangentOffset) = 0;

    virtual int32_t getTangentOffset() const = 0;

    virtual void setTangentOffset(const int32_t tangentOffset) = 0;

    virtual int32_t getTexcoordOffset() const = 0;

    virtual void setTexcoordOffset(const int32_t texcoordOffset) = 0;

    virtual int32_t getBoneIndices0Offset() const = 0;

    virtual void setBoneIndices0Offset(const int32_t boneIndices0Offset) = 0;

    virtual int32_t getBoneIndices1Offset() const = 0;

    virtual void setBoneIndices1Offset(const int32_t boneIndices1Offset) = 0;

    virtual int32_t getBoneWeights0Offset() const = 0;

    virtual void setBoneWeights0Offset(const int32_t boneWeights0Offset) = 0;

    virtual int32_t getBoneWeights1Offset() const = 0;

    virtual void setBoneWeights1Offset(const int32_t boneWeights1Offset) = 0;

    virtual int32_t getNumberBonesOffset() const = 0;

    virtual void setNumberBonesOffset(const int32_t numberBonesOffset) = 0;

    virtual uint32_t getStrideInBytes() const = 0;

    virtual void setStrideInBytes(const uint32_t strideInBytes) = 0;

    virtual VkBool32 hasBones() const = 0;

    virtual void updateDescriptorSetsRecursive(const std::string& nodeName, const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets) = 0;

    virtual void bindIndexBuffer(const ICommandBuffersSP& cmdBuffer, const uint32_t bufferIndex = 0) const = 0;

    virtual void bindVertexBuffers(const ICommandBuffersSP& cmdBuffer, const uint32_t bufferIndex = 0) const = 0;

    virtual void drawIndexed(const ICommandBuffersSP& cmdBuffer, const uint32_t bufferIndex = 0) const = 0;

    virtual void bindDrawIndexedRecursive(const std::string& nodeName, const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const overwrite* renderOverwrite = nullptr, const uint32_t bufferIndex = 0) const = 0;

    virtual const aabb& getAABB() const = 0;

};

typedef std::shared_ptr<ISubMesh> ISubMeshSP;

} /* namespace vkts */

#endif /* VKTS_ISUBMESH_HPP_ */
