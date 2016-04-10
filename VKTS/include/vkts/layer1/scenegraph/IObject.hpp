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

#ifndef VKTS_IOBJECT_HPP_
#define VKTS_IOBJECT_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class overwrite;

class IObject: public ICloneable<IObject>, public IDestroyable
{

public:

    IObject() :
        ICloneable<IObject>(), IDestroyable()
    {
    }

    virtual ~IObject()
    {
    }

    virtual const std::string& getName() const = 0;

    virtual void setName(const std::string& name) = 0;

    virtual const glm::vec3& getTranslate() const = 0;

    virtual void setTranslate(const glm::vec3& translate) = 0;

    virtual const glm::vec3& getRotate() const = 0;

    virtual void setRotate(const glm::vec3& rotate) = 0;

    virtual const glm::vec3& getScale() const = 0;

    virtual void setScale(const glm::vec3& scale) = 0;

    virtual const INodeSP& getRootNode() const = 0;

    virtual void setRootNode(const INodeSP& rootNode) = 0;

    virtual void setDirty() = 0;

    virtual void updateDescriptorSetsRecursive(const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets) = 0;

    virtual void bindDrawIndexedRecursive(const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const overwrite* renderOverwrite = nullptr, const uint32_t bufferIndex = 0) const = 0;

    virtual void updateRecursive(const IUpdateThreadContext& updateContext) = 0;

};

typedef std::shared_ptr<IObject> IObjectSP;

} /* namespace vkts */

#endif /* VKTS_IOBJECT_HPP_ */
