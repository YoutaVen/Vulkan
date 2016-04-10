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

#ifndef VKTS_NODE_HPP_
#define VKTS_NODE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Node: public INode
{

private:

    std::string name;

    INodeSP parentNode;

    glm::vec3 translate;
    glm::vec3 rotate;
    glm::vec3 scale;

    glm::mat4 transformMatrix;
    VkBool32 transformMatrixDirty;

    int32_t jointIndex;
    int32_t joints;

    glm::vec3 bindTranslate;
    glm::vec3 bindRotate;
    glm::vec3 bindScale;

    glm::mat4 bindMatrix;
    glm::mat4 inverseBindMatrix;
    VkBool32 bindMatrixDirty;

    SmartPointerVector<INodeSP> allChildNodes;

    SmartPointerVector<IMeshSP> allMeshes;

    SmartPointerVector<IAnimationSP> allAnimations;

    int32_t currentAnimation;

    float currentTime;

    IBufferObjectSP transformUniformBuffer;

    VkDescriptorBufferInfo transformDescriptorBufferInfo;
    VkWriteDescriptorSet transformWriteDescriptorSet;

    IBufferObjectSP jointsUniformBuffer;

    VkDescriptorBufferInfo jointDescriptorBufferInfo;
    VkWriteDescriptorSet jointWriteDescriptorSet;

    aabb box;

    void reset();

    void updateTransformDescriptorBufferInfo(const VkBuffer buffer, const VkDeviceSize offset, const VkDeviceSize range);

    void updateJointDescriptorBufferInfo(const VkBuffer buffer, const VkDeviceSize offset, const VkDeviceSize range);

public:

    Node();
    Node(const Node& other);
    Node(Node&& other) = delete;
    virtual ~Node();

    Node& operator =(const Node& other) = delete;
    Node& operator =(Node && other) = delete;

    //
    // INode
    //

    virtual const std::string& getName() const override;

    virtual void setName(const std::string& name) override;

    virtual const INodeSP& getParentNode() const override;

    virtual void setParentNode(const INodeSP& parentNode) override;

    virtual const glm::vec3& getTranslate() const override;

    virtual void setTranslate(const glm::vec3& translate) override;

    virtual const glm::vec3& getRotate() const override;

    virtual void setRotate(const glm::vec3& rotate) override;

    virtual const glm::vec3& getScale() const override;

    virtual void setScale(const glm::vec3& scale) override;

    virtual int32_t getJointIndex() const override;

    virtual void setJointIndex(const int32_t jointIndex) override;

    virtual int32_t getNumberJoints() const override;

    virtual const glm::vec3& getBindTranslate() const override;

    virtual void setBindTranslate(const glm::vec3& translate) override;

    virtual const glm::vec3& getBindRotate() const override;

    virtual void setBindRotate(const glm::vec3& rotate) override;

    virtual const glm::vec3& getBindScale() const override;

    virtual void setBindScale(const glm::vec3& scale) override;

    virtual void addChildNode(const INodeSP& childNode) override;

    virtual VkBool32 removeChildNode(const INodeSP& childNode) override;

    virtual size_t getNumberChildNodes() const override;

    virtual const SmartPointerVector<INodeSP>& getChildNodes() const override;

    virtual void addMesh(const IMeshSP& mesh) override;

    virtual VkBool32 removeMesh(const IMeshSP& mesh) override;

    virtual size_t getNumberMeshes() const override;

    virtual const SmartPointerVector<IMeshSP>& getMeshes() const override;

    virtual void addAnimation(const IAnimationSP& animation) override;

    virtual VkBool32 removeAnimation(const IAnimationSP& animation) override;

    virtual size_t getNumberAnimations() const override;

    virtual const SmartPointerVector<IAnimationSP>& getAnimations() const override;

    virtual VkBool32 getDirty() const override;

    virtual void setDirty() override;

    virtual IBufferObjectSP getTransformUniformBuffer() const override;

    virtual void setTransformUniformBuffer(const IBufferObjectSP& transformUniformBuffer)

    override; virtual IBufferObjectSP getJointsUniformBuffer() const override;

    virtual void setJointsUniformBuffer(const int32_t joints, const IBufferObjectSP& jointsUniformBuffer) override;

    virtual void updateDescriptorSetsRecursive(const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets) override;

    virtual void bindDrawIndexedRecursive(const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const overwrite* renderOverwrite = nullptr, const uint32_t bufferIndex = 0) const override;

    virtual void updateRecursive(const IUpdateThreadContext& updateContext, const glm::mat4& parentTransformMatrix, const VkBool32 parentTransformMatrixDirty, const glm::mat4& parentBindMatrix, const VkBool32 parentBindMatrixDirty, const INodeSP& armatureNode) override;

    virtual const aabb& getAABB() const override;

    virtual sphere getBoundingSphere() const override;

    //
    // ICloneable
    //

    virtual INodeSP clone() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_NODE_HPP_ */
