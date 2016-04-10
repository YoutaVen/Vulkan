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

#include "Object.hpp"

#include "Node.hpp"

namespace vkts
{

Object::Object() :
    IObject(), name(""), translate(0.0f, 0.0f, 0.0f), rotate(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), transformMatrix(1.0f), dirty(VK_TRUE), rootNode()
{
}

Object::Object(const Object& other) :
    IObject(), name(other.name), translate(other.translate), rotate(other.rotate), scale(other.scale), transformMatrix(other.transformMatrix), dirty(VK_TRUE), rootNode()
{
    if (!other.rootNode.get())
    {
        name = "";

        translate = glm::vec3(0.0f, 0.0f, 0.0f);
        rotate = glm::vec3(0.0f, 0.0f, 0.0f);
        scale = glm::vec3(1.0f, 1.0f, 1.0f);

        return;
    }

    rootNode = other.rootNode->clone();

    if (!rootNode.get())
    {
        name = "";

        translate = glm::vec3(0.0f, 0.0f, 0.0f);
        rotate = glm::vec3(0.0f, 0.0f, 0.0f);
        scale = glm::vec3(1.0f, 1.0f, 1.0f);

        return;
    }
}

Object::~Object()
{
    destroy();
}

//
// IObject
//

const std::string& Object::getName() const
{
    return name;
}

void Object::setName(const std::string& name)
{
    this->name = name;
}

const glm::vec3& Object::getTranslate() const
{
    return translate;
}

void Object::setTranslate(const glm::vec3& translate)
{
    this->translate = translate;

    setDirty();
}

const glm::vec3& Object::getRotate() const
{
    return rotate;
}

void Object::setRotate(const glm::vec3& rotate)
{
    this->rotate = rotate;

    setDirty();
}

const glm::vec3& Object::getScale() const
{
    return scale;
}

const INodeSP& Object::getRootNode() const
{
    return rootNode;
}

void Object::setRootNode(const INodeSP& rootNode)
{
    this->rootNode = rootNode;

    setDirty();
}

void Object::setScale(const glm::vec3& scale)
{
    this->scale = scale;

    setDirty();
}

void Object::setDirty()
{
    dirty = VK_TRUE;
}

void Object::updateDescriptorSetsRecursive(const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets)
{
    if (rootNode.get())
    {
        rootNode->updateDescriptorSetsRecursive(allWriteDescriptorSetsCount, allWriteDescriptorSets);
    }
}

void Object::bindDrawIndexedRecursive(const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const overwrite* renderOverwrite, const uint32_t bufferIndex) const
{
    const overwrite* currentOverwrite = renderOverwrite;
    while (currentOverwrite)
    {
    	if (!currentOverwrite->objectBindDrawIndexedRecursive(*this, cmdBuffer, allGraphicsPipelines, bufferIndex))
    	{
    		return;
    	}

    	currentOverwrite = currentOverwrite->getNextOverwrite();
    }

    if (rootNode.get())
    {
        rootNode->bindDrawIndexedRecursive(cmdBuffer, allGraphicsPipelines, renderOverwrite, bufferIndex);
    }
}

void Object::updateRecursive(const IUpdateThreadContext& updateContext)
{
    if (dirty)
    {
        transformMatrix = translateMat4(translate.x, translate.y, translate.z) * rotateRzRyRxMat4(rotate.z, rotate.y, rotate.x) * scaleMat4(scale.x, scale.y, scale.z);

        dirty = VK_FALSE;
    }

    if (rootNode.get())
    {
        rootNode->updateRecursive(updateContext, transformMatrix, dirty, glm::mat4(1.0f), VK_FALSE, INodeSP());
    }
}

//
// ICloneable
//

IObjectSP Object::clone() const
{
    return IObjectSP(new Object(*this));
}

//
// IDestroyable
//

void Object::destroy()
{
    if (rootNode.get())
    {
        rootNode->destroy();
    }
}

} /* namespace vkts */
