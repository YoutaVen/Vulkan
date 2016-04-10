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

#ifndef VKTS_MESH_HPP_
#define VKTS_MESH_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Mesh: public IMesh
{

private:

    std::string name;

    SmartPointerVector<ISubMeshSP> allSubMeshes;

    glm::vec2 displace;

    aabb box;

public:

    Mesh();
    Mesh(const Mesh& other);
    Mesh(Mesh&& other) = delete;
    virtual ~Mesh();

    Mesh& operator =(const Mesh& other) = delete;
    Mesh& operator =(Mesh && other) = delete;

    //
    // IMesh
    //

    virtual const std::string& getName() const override;

    virtual void setName(const std::string& name) override;

    virtual void addSubMesh(const ISubMeshSP& subMesh) override;

    virtual VkBool32 removeSubMesh(const ISubMeshSP& subMesh) override;

    virtual size_t getNumberSubMeshes() const override;

    virtual const SmartPointerVector<ISubMeshSP>& getSubMeshes() const override;

    virtual void updateDescriptorSetsRecursive(const std::string& nodeName, const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets) override;

    virtual void bindDrawIndexedRecursive(const std::string& nodeName, const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const overwrite* renderOverwrite = nullptr, const uint32_t bufferIndex = 0) const override;

    virtual void setDisplace(const glm::vec2& displace) override;

    virtual const glm::vec2& getDisplace() const override;

    virtual void setAABB(const aabb& box) override;

    virtual const aabb& getAABB() const override;

    //
    // ICloneable
    //

    virtual IMeshSP clone() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_MESH_HPP_ */
