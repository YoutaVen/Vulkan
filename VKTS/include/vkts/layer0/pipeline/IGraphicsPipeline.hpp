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

#ifndef VKTS_IGRAPHICSPIPELINE_HPP_
#define VKTS_IGRAPHICSPIPELINE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class IGraphicsPipeline: public IDestroyable
{

public:

    IGraphicsPipeline() :
        IDestroyable()
    {
    }

    virtual ~IGraphicsPipeline()
    {
    }

    virtual const VkDevice getDevice() const = 0;

    virtual const VkGraphicsPipelineCreateInfo& getGraphicsPipelineCreateInfo() const = 0;

    virtual VkPipelineCreateFlags getFlags() const = 0;

    virtual uint32_t getStageCount() const = 0;

    virtual const VkPipelineShaderStageCreateInfo* getStages() const = 0;

    virtual const VkPipelineVertexInputStateCreateInfo* getVertexInputState() const = 0;

    virtual const VkPipelineInputAssemblyStateCreateInfo* getInputAssemblyState() const = 0;

    virtual const VkPipelineTessellationStateCreateInfo* getTessellationState() const = 0;

    virtual const VkPipelineViewportStateCreateInfo* getViewportState() const = 0;

    virtual const VkPipelineRasterizationStateCreateInfo* getRasterizationState() const = 0;

    virtual const VkPipelineMultisampleStateCreateInfo* getMultisampleState() const = 0;

    virtual const VkPipelineDepthStencilStateCreateInfo* getDepthStencilState() const = 0;

    virtual const VkPipelineColorBlendStateCreateInfo* getColorBlendState() const = 0;

    virtual const VkPipelineDynamicStateCreateInfo* getDynamicState() const = 0;

    virtual const VkPipelineLayout getLayout() const = 0;

    virtual const VkRenderPass getRenderPass() const = 0;

    virtual uint32_t getSubpass() const = 0;

    virtual const VkPipeline getBasePipelineHandle() const = 0;

    virtual int32_t getBasePipelineIndex() const = 0;

    virtual const VkPipeline getPipeline() const = 0;

    virtual VkTsVertexBufferType getVertexBufferType() const = 0;

};

typedef std::shared_ptr<IGraphicsPipeline> IGraphicsPipelineSP;

} /* namespace vkts */

#endif /* VKTS_IGRAPHICSPIPELINE_HPP_ */
