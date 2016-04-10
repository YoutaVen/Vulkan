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

#ifndef VKTS_INODE_HPP_
#define VKTS_INODE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class overwrite;

class INode: public ICloneable<INode>, public IDestroyable, public std::enable_shared_from_this<INode>
{

public:

    INode() :
        ICloneable<INode>(), IDestroyable()
    {
    }

    virtual ~INode()
    {
    }

    virtual const std::string& getName() const = 0;

    virtual void setName(const std::string& name) = 0;

    virtual const std::shared_ptr<INode>& getParentNode() const = 0;

    virtual void setParentNode(const std::shared_ptr<INode>& parentNode) = 0;

    virtual const glm::vec3& getTranslate() const = 0;

    virtual void setTranslate(const glm::vec3& translate) = 0;

    virtual const glm::vec3& getRotate() const = 0;

    virtual void setRotate(const glm::vec3& rotate) = 0;

    virtual const glm::vec3& getScale() const = 0;

    virtual void setScale(const glm::vec3& scale) = 0;

    virtual int32_t getJointIndex() const = 0;

    virtual void setJointIndex(const int32_t jointIndex) = 0;

    virtual int32_t getNumberJoints() const = 0;

    virtual const glm::vec3& getBindTranslate() const = 0;

    virtual void setBindTranslate(const glm::vec3& translate) = 0;

    virtual const glm::vec3& getBindRotate() const = 0;

    virtual void setBindRotate(const glm::vec3& rotate) = 0;

    virtual const glm::vec3& getBindScale() const = 0;

    virtual void setBindScale(const glm::vec3& scale) = 0;

    virtual void addChildNode(const std::shared_ptr<INode>& childNode) = 0;

    virtual VkBool32 removeChildNode(const std::shared_ptr<INode>& childNode) = 0;

    virtual size_t getNumberChildNodes() const = 0;

    virtual const SmartPointerVector<std::shared_ptr<INode>>& getChildNodes() const = 0;

    virtual void addMesh(const IMeshSP& mesh) = 0;

    virtual VkBool32 removeMesh(const IMeshSP& mesh) = 0;

    virtual size_t getNumberMeshes() const = 0;

    virtual const SmartPointerVector<IMeshSP>& getMeshes() const = 0;

    virtual void addAnimation(const IAnimationSP& animation) = 0;

    virtual VkBool32 removeAnimation(const IAnimationSP& animation) = 0;

    virtual size_t getNumberAnimations() const = 0;

    virtual const SmartPointerVector<IAnimationSP>& getAnimations() const = 0;

    virtual VkBool32 getDirty() const = 0;

    virtual void setDirty() = 0;

    virtual IBufferObjectSP getTransformUniformBuffer() const = 0;

    virtual void setTransformUniformBuffer(const IBufferObjectSP& transformUniformBuffer) = 0;

    virtual IBufferObjectSP getJointsUniformBuffer() const = 0;

    virtual void setJointsUniformBuffer(const int32_t joints, const IBufferObjectSP& jointsUniformBuffer) = 0;

    virtual void updateDescriptorSetsRecursive(const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets) = 0;

    virtual void bindDrawIndexedRecursive(const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const overwrite* renderOverwrite = nullptr, const uint32_t bufferIndex = 0) const = 0;

    virtual void updateRecursive(const IUpdateThreadContext& updateContext, const glm::mat4& parentTransformMatrix, const VkBool32 parentTransformMatrixDirty, const glm::mat4& parentBindMatrix, const VkBool32 parentBindMatrixDirty, const std::shared_ptr<INode>& armatureNode) = 0;

    virtual const aabb& getAABB() const = 0;

    virtual sphere getBoundingSphere() const = 0;

};

typedef std::shared_ptr<INode> INodeSP;

} /* namespace vkts */

#endif /* VKTS_INODE_HPP_ */
