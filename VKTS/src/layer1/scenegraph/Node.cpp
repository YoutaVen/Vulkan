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

#include "Node.hpp"

#include "Animation.hpp"
#include "Mesh.hpp"

namespace vkts
{

void Node::reset()
{
    name = "";

    parentNode = INodeSP();

    translate = glm::vec3(0.0f, 0.0f, 0.0f);
    rotate = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);

    transformMatrix = glm::mat4(1.0f);
    transformMatrixDirty = VK_TRUE;

    jointIndex = -1;
    joints = 0;

    bindTranslate = glm::vec3(0.0f, 0.0f, 0.0f);
    bindRotate = glm::vec3(0.0f, 0.0f, 0.0f);
    bindScale = glm::vec3(1.0f, 1.0f, 1.0f);

    bindMatrix = glm::mat4(1.0f);
    inverseBindMatrix = glm::mat4(1.0f);

    bindMatrixDirty = VK_FALSE;

    allChildNodes.clear();

    allMeshes.clear();

    allAnimations.clear();

    currentAnimation = -1;

    currentTime = 0.0f;

    transformUniformBuffer = IBufferObjectSP();

    memset(&transformDescriptorBufferInfo, 0, sizeof(transformDescriptorBufferInfo));
    memset(&transformWriteDescriptorSet, 0, sizeof(transformWriteDescriptorSet));

    jointsUniformBuffer = IBufferObjectSP();
    memset(&jointDescriptorBufferInfo, 0, sizeof(jointDescriptorBufferInfo));
    memset(&jointWriteDescriptorSet, 0, sizeof(jointWriteDescriptorSet));

    box = aabb(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

Node::Node() :
    INode(), name(""), parentNode(), translate(0.0f, 0.0f, 0.0f), rotate(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), transformMatrix(1.0f), transformMatrixDirty(VK_TRUE), jointIndex(-1), joints(0), bindTranslate(0.0f, 0.0f, 0.0f), bindRotate(0.0f, 0.0f,0.0f), bindScale(1.0f, 1.0f, 1.0f), bindMatrix(1.0f), inverseBindMatrix(1.0f), bindMatrixDirty(VK_FALSE), allChildNodes(), allMeshes(), allAnimations(), currentAnimation(-1), currentTime(0.0f), transformUniformBuffer(), jointsUniformBuffer(), box(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
{
    reset();
}

Node::Node(const Node& other) :
    INode(), name(other.name), parentNode(other.parentNode), translate(other.translate), rotate(other.rotate), scale(other.scale), transformMatrix(other.transformMatrix), transformMatrixDirty(other.transformMatrixDirty), jointIndex(-1), joints(0), bindTranslate(other.bindTranslate), bindRotate(other.bindRotate), bindScale(other.bindScale), bindMatrix(other.bindMatrix), inverseBindMatrix(other.inverseBindMatrix), bindMatrixDirty(other.bindMatrixDirty), box(other.box)
{
    for (size_t i = 0; i < other.allChildNodes.size(); i++)
    {
        const auto& currentChildNode = other.allChildNodes[i];

        if (!currentChildNode.get())
        {
            reset();

            break;
        }

        INodeSP cloneChildNode = currentChildNode->clone();

        if (!cloneChildNode.get())
        {
            reset();

            break;
        }

        allChildNodes.append(cloneChildNode);
    }

    for (size_t i = 0; i < other.allMeshes.size(); i++)
    {
        const auto& currentMesh = other.allMeshes[i];

        if (!currentMesh.get())
        {
            reset();

            break;
        }

        IMeshSP cloneMesh = currentMesh->clone();

        if (!cloneMesh.get())
        {
            reset();

            break;
        }

        allMeshes.append(cloneMesh);
    }

    for (size_t i = 0; i < other.allAnimations.size(); i++)
    {
        const auto& currentAnimation = other.allAnimations[i];

        if (!currentAnimation.get())
        {
            reset();

            break;
        }

        IAnimationSP cloneAnimation = currentAnimation->clone();

        if (!cloneAnimation.get())
        {
            reset();

            break;
        }

        allAnimations.append(cloneAnimation);
    }

    currentAnimation = other.currentAnimation;

    currentTime = other.currentTime;

    //

    if (other.transformUniformBuffer.get())
    {
		IBufferObjectSP transformUniformBuffer = bufferObjectCreate(other.transformUniformBuffer->getInitialResources(), other.transformUniformBuffer->getBuffer()->getBufferCreateInfo(), other.transformUniformBuffer->getDeviceMemory()->getMemoryPropertyFlags());

		if (!transformUniformBuffer.get())
		{
			reset();

			return;
		}

		setTransformUniformBuffer(transformUniformBuffer);
    }

    //

    setJointIndex(other.jointIndex);

    //

    if (other.jointsUniformBuffer.get())
    {

        IBufferObjectSP jointsUniformBuffer = bufferObjectCreate(other.jointsUniformBuffer->getInitialResources(), other.jointsUniformBuffer->getBuffer()->getBufferCreateInfo(), other.jointsUniformBuffer->getDeviceMemory()->getMemoryPropertyFlags());

        if (!jointsUniformBuffer.get())
        {
            reset();

            return;
        }

        setJointsUniformBuffer(other.joints, jointsUniformBuffer);
    }
}

Node::~Node()
{
    destroy();
}

void Node::updateTransformDescriptorBufferInfo(const VkBuffer buffer, const VkDeviceSize offset, const VkDeviceSize range)
{
    memset(&transformDescriptorBufferInfo, 0, sizeof(VkDescriptorBufferInfo));

    transformDescriptorBufferInfo.buffer = buffer;
    transformDescriptorBufferInfo.offset = offset;
    transformDescriptorBufferInfo.range = range;

    memset(&transformWriteDescriptorSet, 0, sizeof(VkWriteDescriptorSet));

    transformWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    transformWriteDescriptorSet.dstSet = VK_NULL_HANDLE; // Defined later.
    transformWriteDescriptorSet.dstBinding = VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM;
    transformWriteDescriptorSet.dstArrayElement = 0;
    transformWriteDescriptorSet.descriptorCount = 1;
    transformWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    transformWriteDescriptorSet.pImageInfo = nullptr;
    transformWriteDescriptorSet.pBufferInfo = &transformDescriptorBufferInfo;
    transformWriteDescriptorSet.pTexelBufferView = nullptr;
}

void Node::updateJointDescriptorBufferInfo(const VkBuffer buffer, const VkDeviceSize offset, const VkDeviceSize range)
{
    memset(&jointDescriptorBufferInfo, 0, sizeof(VkDescriptorBufferInfo));

    jointDescriptorBufferInfo.buffer = buffer;
    jointDescriptorBufferInfo.offset = offset;
    jointDescriptorBufferInfo.range = range;

    memset(&jointWriteDescriptorSet, 0, sizeof(VkWriteDescriptorSet));

    jointWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    jointWriteDescriptorSet.dstSet = VK_NULL_HANDLE; // Defined later.
    jointWriteDescriptorSet.dstBinding = VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM;
    jointWriteDescriptorSet.dstArrayElement = 0;
    jointWriteDescriptorSet.descriptorCount = 1;
    jointWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    jointWriteDescriptorSet.pImageInfo = nullptr;
    jointWriteDescriptorSet.pBufferInfo = &jointDescriptorBufferInfo;
    jointWriteDescriptorSet.pTexelBufferView = nullptr;
}

//
// INode
//

const std::string& Node::getName() const
{
    return name;
}

void Node::setName(const std::string& name)
{
    this->name = name;
}

const INodeSP& Node::getParentNode() const
{
    return parentNode;
}

void Node::setParentNode(const INodeSP& parentNode)
{
    this->parentNode = parentNode;
}

const glm::vec3& Node::getTranslate() const
{
    return translate;
}

void Node::setTranslate(const glm::vec3& translate)
{
    this->translate = translate;

    setDirty();
}

const glm::vec3& Node::getRotate() const
{
    return rotate;
}

void Node::setRotate(const glm::vec3& rotate)
{
    this->rotate = rotate;

    setDirty();
}

const glm::vec3& Node::getScale() const
{
    return scale;
}

void Node::setScale(const glm::vec3& scale)
{
    this->scale = scale;

    setDirty();
}

int32_t Node::getJointIndex() const
{
    return jointIndex;
}

void Node::setJointIndex(const int32_t jointIndex)
{
    this->jointIndex = jointIndex;
}

int32_t Node::getNumberJoints() const
{
	return joints;
}

const glm::vec3& Node::getBindTranslate() const
{
    return bindTranslate;
}

void Node::setBindTranslate(const glm::vec3& translate)
{
    this->bindTranslate = translate;

    this->bindMatrixDirty = VK_TRUE;
}

const glm::vec3& Node::getBindRotate() const
{
    return bindRotate;
}

void Node::setBindRotate(const glm::vec3& rotate)
{
    this->bindRotate = rotate;

    this->bindMatrixDirty = VK_TRUE;
}

const glm::vec3& Node::getBindScale() const
{
    return bindScale;
}

void Node::setBindScale(const glm::vec3& scale)
{
    this->bindScale = scale;

    this->bindMatrixDirty = VK_TRUE;
}

void Node::addChildNode(const INodeSP& childNode)
{
    allChildNodes.append(childNode);

    //

    if (allChildNodes.size() == 1)
    {
    	this->box = childNode->getAABB();
    }
    else
    {
    	this->box += childNode->getAABB();
    }
}

VkBool32 Node::removeChildNode(const INodeSP& childNode)
{
    return allChildNodes.remove(childNode);
}

size_t Node::getNumberChildNodes() const
{
    return allChildNodes.size();
}

const SmartPointerVector<INodeSP>& Node::getChildNodes() const
{
    return allChildNodes;
}

void Node::addMesh(const IMeshSP& mesh)
{
    allMeshes.append(mesh);

    //

    if (allMeshes.size() == 1)
    {
    	this->box = mesh->getAABB();
    }
    else
    {
    	this->box += mesh->getAABB();
    }
}

VkBool32 Node::removeMesh(const IMeshSP& mesh)
{
    return allMeshes.remove(mesh);
}

size_t Node::getNumberMeshes() const
{
    return allMeshes.size();
}

const SmartPointerVector<IMeshSP>& Node::getMeshes() const
{
    return allMeshes;
}

void Node::addAnimation(const IAnimationSP& animation)
{
    allAnimations.append(animation);

    if (allAnimations.size() == 1)
    {
        currentAnimation = 0;
    }
}

VkBool32 Node::removeAnimation(const IAnimationSP& animation)
{
    VkBool32 result = allAnimations.remove(animation);

    if (currentAnimation >= (int32_t) allAnimations.size())
    {
        currentAnimation = (int32_t) allAnimations.size() - 1;
    }

    return result;
}

size_t Node::getNumberAnimations() const
{
    return allAnimations.size();
}

const SmartPointerVector<IAnimationSP>& Node::getAnimations() const
{
    return allAnimations;
}

VkBool32 Node::getDirty() const
{
    return transformMatrixDirty;
}

void Node::setDirty()
{
    transformMatrixDirty = VK_TRUE;
}

IBufferObjectSP Node::getTransformUniformBuffer() const
{
    return transformUniformBuffer;
}

void Node::setTransformUniformBuffer(const IBufferObjectSP& transformUniformBuffer)
{
    this->transformUniformBuffer = transformUniformBuffer;

    updateTransformDescriptorBufferInfo(this->transformUniformBuffer->getBuffer()->getBuffer(), 0, this->transformUniformBuffer->getBuffer()->getSize());

    setDirty();
}

IBufferObjectSP Node::getJointsUniformBuffer() const
{
	return jointsUniformBuffer;
}

void Node::setJointsUniformBuffer(const int32_t joints, const IBufferObjectSP& jointsUniformBuffer)
{
	this->joints = joints;
	this->jointsUniformBuffer = jointsUniformBuffer;

    updateJointDescriptorBufferInfo(this->jointsUniformBuffer->getBuffer()->getBuffer(), 0, this->jointsUniformBuffer->getBuffer()->getSize());

    this->bindMatrixDirty = VK_TRUE;
}

void Node::updateDescriptorSetsRecursive(const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets)
{
	if (transformWriteDescriptorSet.dstBinding == VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM)
	{
		allWriteDescriptorSets[transformWriteDescriptorSet.dstBinding] = transformWriteDescriptorSet;
	}

	if (jointWriteDescriptorSet.dstBinding == VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM)
	{
		allWriteDescriptorSets[jointWriteDescriptorSet.dstBinding] = jointWriteDescriptorSet;
	}

    if (allMeshes.size() > 0)
    {
        for (size_t i = 0; i < allMeshes.size(); i++)
        {
            allMeshes[i]->updateDescriptorSetsRecursive(name, allWriteDescriptorSetsCount, allWriteDescriptorSets);
        }
    }

    for (size_t i = 0; i < allChildNodes.size(); i++)
    {
        allChildNodes[i]->updateDescriptorSetsRecursive(allWriteDescriptorSetsCount, allWriteDescriptorSets);
    }
}

void Node::bindDrawIndexedRecursive(const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const overwrite* renderOverwrite, const uint32_t bufferIndex) const
{
    const overwrite* currentOverwrite = renderOverwrite;
    while (currentOverwrite)
    {
    	if (!currentOverwrite->nodeBindDrawIndexedRecursive(*this, cmdBuffer, allGraphicsPipelines, bufferIndex))
    	{
    		return;
    	}

    	currentOverwrite = currentOverwrite->getNextOverwrite();
    }

    for (size_t i = 0; i < allMeshes.size(); i++)
    {
        allMeshes[i]->bindDrawIndexedRecursive(name, cmdBuffer, allGraphicsPipelines, renderOverwrite, bufferIndex);
    }

    for (size_t i = 0; i < allChildNodes.size(); i++)
    {
        allChildNodes[i]->bindDrawIndexedRecursive(cmdBuffer, allGraphicsPipelines, renderOverwrite, bufferIndex);
    }
}

void Node::updateRecursive(const IUpdateThreadContext& updateContext, const glm::mat4& parentTransformMatrix, const VkBool32 parentTransformMatrixDirty, const glm::mat4& parentBindMatrix, const VkBool32 parentBindMatrixDirty, const INodeSP& armatureNode)
{
    transformMatrixDirty = transformMatrixDirty || parentTransformMatrixDirty;

    bindMatrixDirty = bindMatrixDirty || parentBindMatrixDirty;

    auto newArmatureNode = (joints != 0) ? INode::shared_from_this() : armatureNode;

    if (currentAnimation >= 0 && currentAnimation < (int32_t) allAnimations.size())
    {
        currentTime += (float) updateContext.getDeltaTime();

        if (currentTime < allAnimations[currentAnimation]->getStart() || currentTime > allAnimations[currentAnimation]->getStop())
        {
            currentTime = allAnimations[currentAnimation]->getStart();
        }

        const auto& currentChannels = allAnimations[currentAnimation]->getChannels();

        float value;

        //

        quat quaternion;

        VkBool32 quaternionDirty = VK_FALSE;

        //

        for (size_t i = 0; i < currentChannels.size(); i++)
        {
            value = interpolate(currentTime, currentChannels[i]);

            if (currentChannels[i]->getTargetTransform() == VKTS_TARGET_TRANSFORM_TRANSLATE)
            {
                translate[currentChannels[i]->getTargetTransformElement()] = value;
            }
            else if (currentChannels[i]->getTargetTransform() == VKTS_TARGET_TRANSFORM_ROTATE)
            {
                rotate[currentChannels[i]->getTargetTransformElement()] = value;
            }
            else if (currentChannels[i]->getTargetTransform() == VKTS_TARGET_TRANSFORM_QUATERNION_ROTATE)
            {
            	switch (currentChannels[i]->getTargetTransformElement())
            	{
            		case VKTS_TARGET_TRANSFORM_ELEMENT_X:
            			quaternion.x = value;
            			break;
            		case VKTS_TARGET_TRANSFORM_ELEMENT_Y:
            			quaternion.y = value;
            			break;
            		case VKTS_TARGET_TRANSFORM_ELEMENT_Z:
            			quaternion.z = value;
            			break;
            		case VKTS_TARGET_TRANSFORM_ELEMENT_W:
            			quaternion.w = value;
            			break;
            	}

                quaternionDirty = VK_TRUE;
            }
            else if (currentChannels[i]->getTargetTransform() == VKTS_TARGET_TRANSFORM_SCALE)
            {
                scale[currentChannels[i]->getTargetTransformElement()] = value;
            }
        }

        if (quaternionDirty)
        {
        	rotate = quaternion.rotation();
        }

        //

        transformMatrixDirty = VK_TRUE;

        if (joints != 0)
        {
        	bindMatrixDirty = VK_TRUE;
        }
    }

    //

    if (bindMatrixDirty)
    {
    	// Only armature, having joints not zero and joints can enter here.

    	glm::mat4 localBindMatrix;

    	if (joints == 0)
    	{
    		localBindMatrix = translateMat4(bindTranslate.x, bindTranslate.y, bindTranslate.z) * rotateRzRyRxMat4(bindRotate.z, bindRotate.y, bindRotate.x) * scaleMat4(bindScale.x, bindScale.y, bindScale.z);
    	}
    	else
    	{
    		// Armature has no bind values, but transform is taken into account.

    		localBindMatrix = translateMat4(translate.x, translate.y, translate.z) * rotateRzRyRxMat4(rotate.z, rotate.y, rotate.x) * scaleMat4(scale.x, scale.y, scale.z);
    	}

		this->bindMatrix = parentBindMatrix * localBindMatrix;

        this->inverseBindMatrix = glm::inverse(this->bindMatrix);

        //

        transformMatrixDirty = VK_TRUE;
    }

    //

    if (transformMatrixDirty)
    {
        if (jointIndex == -1)
        {
        	this->transformMatrix = translateMat4(translate.x, translate.y, translate.z) * rotateRzRyRxMat4(rotate.z, rotate.y, rotate.x) * scaleMat4(scale.x, scale.y, scale.z);

        	// Only use parent transform, if this is not an armature.
        	if (joints == 0)
        	{
        		this->transformMatrix = parentTransformMatrix * this->transformMatrix;
        	}
        }
        else
        {
        	// Processing joints.

        	this->transformMatrix = bindMatrix * translateMat4(translate.x, translate.y, translate.z) * rotateRzRyRxMat4(rotate.z, rotate.y, rotate.x) * scaleMat4(scale.x, scale.y, scale.z) * this->inverseBindMatrix;

        	// Only use parent transform, if parent is not an armature.
        	if (parentNode.get() && parentNode->getNumberJoints() == 0)
        	{
        		this->transformMatrix = parentTransformMatrix * this->transformMatrix;
        	}
        }

        // Update buffer.
        if (allMeshes.size() > 0)
        {
            transformUniformBuffer->upload(0, 0, this->transformMatrix);

            auto transformNormalMatrix = glm::transpose(glm::inverse(glm::mat3(this->transformMatrix)));

            transformUniformBuffer->upload(sizeof(float) * 16, 0, transformNormalMatrix);
        }
        else if (joints != 0)
        {
        	// If this is an armature, store the parent matrix.
        	// This allows to modify the parent matrices without recalculating the bind matrices.

			jointsUniformBuffer->upload(0, 0, parentTransformMatrix);

            auto transformNormalMatrix = glm::transpose(glm::inverse(glm::mat3(parentTransformMatrix)));

            jointsUniformBuffer->upload(sizeof(float) * 16, 0, transformNormalMatrix);
        }
        else if (jointIndex >= 0 && jointIndex < VKTS_MAX_JOINTS)
        {
        	if (newArmatureNode.get())
        	{
        		auto currentJointsUniformBuffer = newArmatureNode->getJointsUniformBuffer();

        		if (currentJointsUniformBuffer.get())
        		{
        			// Upload the joint matrices to blend them on the GPU.

        			size_t offset = sizeof(float) * 16 + sizeof(float) * 12;

        			currentJointsUniformBuffer->upload(offset + jointIndex * sizeof(float) * 16, 0, this->transformMatrix);

                    auto transformNormalMatrix = glm::transpose(glm::inverse(glm::mat3(this->transformMatrix)));

        			currentJointsUniformBuffer->upload(offset + VKTS_MAX_JOINTS * sizeof(float) * 16 + jointIndex * sizeof(float) * 12, 0, transformNormalMatrix);
        		}
        	}
        }
        else if (jointIndex >= VKTS_MAX_JOINTS)
        {
        	logPrint(VKTS_LOG_ERROR, "Node: Too many joints: %d >= %d",  jointIndex, VKTS_MAX_JOINTS);

        	return;
        }
    }

    for (size_t i = 0; i < allChildNodes.size(); i++)
    {
        allChildNodes[i]->updateRecursive(updateContext, this->transformMatrix, this->transformMatrixDirty, this->bindMatrix, this->bindMatrixDirty, newArmatureNode);
    }

    //

    transformMatrixDirty = VK_FALSE;

    bindMatrixDirty = VK_FALSE;
}

const aabb& Node::getAABB() const
{
	return box;
}

sphere Node::getBoundingSphere() const
{
	sphere boundingSphere = transformMatrix * box.getSphere();

	for (size_t i = 0; i < allChildNodes.size(); i++)
    {
		boundingSphere += allChildNodes[i]->getBoundingSphere();
    }

	return boundingSphere;
}

//
// ICloneable
//

INodeSP Node::clone() const
{
    return INodeSP(new Node(*this));
}

//
// IDestroyable
//

void Node::destroy()
{
    for (size_t i = 0; i < allChildNodes.size(); i++)
    {
        allChildNodes[i]->destroy();
    }
    allChildNodes.clear();

    for (size_t i = 0; i < allMeshes.size(); i++)
    {
        allMeshes[i]->destroy();
    }
    allMeshes.clear();

    for (size_t i = 0; i < allAnimations.size(); i++)
    {
        allAnimations[i]->destroy();
    }
    allAnimations.clear();
}

} /* namespace vkts */
