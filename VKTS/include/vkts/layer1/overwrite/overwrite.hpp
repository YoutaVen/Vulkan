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

#ifndef VKTS_OVERWRITE_HPP_
#define VKTS_OVERWRITE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class overwrite
{

private:

	const overwrite* nextOverwrite;

public:

	overwrite() :
		nextOverwrite(nullptr)
    {
    }

	overwrite(const overwrite* nextOverwrite) :
		nextOverwrite(nextOverwrite)
    {
    }

    virtual ~overwrite()
    {
    }

    //

	const overwrite* getNextOverwrite() const
	{
		return nextOverwrite;
	}

	void setNextOverwrite(const overwrite* nextOverwrite)
	{
		this->nextOverwrite = nextOverwrite;
	}

    //

    virtual VkBool32 sceneBindDrawIndexedRecursive(const IScene& scene, const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const uint32_t bufferIndex, const uint32_t objectOffset, const uint32_t objectStep) const
    {
    	return VK_TRUE;
    }

    virtual VkBool32 objectBindDrawIndexedRecursive(const IObject& object, const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const uint32_t bufferIndex) const
    {
    	return VK_TRUE;
    }

    virtual VkBool32 nodeBindDrawIndexedRecursive(const INode& node, const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const uint32_t bufferIndex) const
    {
    	return VK_TRUE;
    }

    virtual VkBool32 meshBindDrawIndexedRecursive(const IMesh& mesh, const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const uint32_t bufferIndex) const
    {
    	return VK_TRUE;
    }

    virtual VkBool32 submeshBindDrawIndexedRecursive(const ISubMesh& subMesh, const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const uint32_t bufferIndex) const
    {
    	return VK_TRUE;
    }

    virtual VkBool32 materialBindDrawIndexedRecursive(const IPhongMaterial& material, const ICommandBuffersSP& cmdBuffer, const IGraphicsPipelineSP& graphicsPipeline, const uint32_t bufferIndex) const
    {
    	return VK_TRUE;
    }

    virtual VkBool32 materialBindDrawIndexedRecursive(const IBSDFMaterial& material, const ICommandBuffersSP& cmdBuffer, const IGraphicsPipelineSP& graphicsPipeline, const uint32_t bufferIndex) const
    {
    	return VK_TRUE;
    }
};

} /* namespace vkts */

#endif /* VKTS_OVERWRITE_HPP_ */
