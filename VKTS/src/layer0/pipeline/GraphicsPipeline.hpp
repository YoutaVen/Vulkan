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

#ifndef VKTS_GRAPHICSPIPELINE_HPP_
#define VKTS_GRAPHICSPIPELINE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class GraphicsPipeline: public IGraphicsPipeline
{

private:

    const VkDevice device;

    VkPipelineCache pipelineCache;

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo;

    SmartPointerVector<IBinaryBufferSP> allStagesSerialized;
    IBinaryBufferSP vertexInputStateSerialized;
    IBinaryBufferSP inputAssemblyStateSerialized;
    IBinaryBufferSP tessellationStateSerialized;
    IBinaryBufferSP viewportStateSerialized;
    IBinaryBufferSP rasterizationStateSerialized;
    IBinaryBufferSP multisampleStateSerialized;
    IBinaryBufferSP depthStencilStateSerialized;
    IBinaryBufferSP colorBlendStateSerialized;
    IBinaryBufferSP dynamicStateSerialized;

    VkPipeline pipeline;

    VkTsVertexBufferType vertexBufferType;

public:

    GraphicsPipeline() = delete;
    GraphicsPipeline(const VkDevice device, const VkPipelineCache pipelineCache, const VkGraphicsPipelineCreateInfo& graphicsPipelineCreateInfo, const VkPipeline pipeline, const VkTsVertexBufferType vertexBufferType);
    GraphicsPipeline(const GraphicsPipeline& other) = delete;
    GraphicsPipeline(GraphicsPipeline&& other) = delete;
    virtual ~GraphicsPipeline();

    GraphicsPipeline& operator =(const GraphicsPipeline& other) = delete;

    GraphicsPipeline& operator =(GraphicsPipeline && other) = delete;

    //
    // IGraphicsPipeline
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkGraphicsPipelineCreateInfo& getGraphicsPipelineCreateInfo() const override;

    virtual VkPipelineCreateFlags getFlags() const override;

    virtual uint32_t getStageCount() const override;

    virtual const VkPipelineShaderStageCreateInfo* getStages() const override;

    virtual const VkPipelineVertexInputStateCreateInfo* getVertexInputState() const override;

    virtual const VkPipelineInputAssemblyStateCreateInfo* getInputAssemblyState() const override;

    virtual const VkPipelineTessellationStateCreateInfo* getTessellationState() const override;

    virtual const VkPipelineViewportStateCreateInfo* getViewportState() const override;

    virtual const VkPipelineRasterizationStateCreateInfo* getRasterizationState() const override;

    virtual const VkPipelineMultisampleStateCreateInfo* getMultisampleState() const override;

    virtual const VkPipelineDepthStencilStateCreateInfo* getDepthStencilState() const override;

    virtual const VkPipelineColorBlendStateCreateInfo* getColorBlendState() const override;

    virtual const VkPipelineDynamicStateCreateInfo* getDynamicState() const override;

    virtual const VkPipelineLayout getLayout() const override;

    virtual const VkRenderPass getRenderPass() const override;

    virtual uint32_t getSubpass() const override;

    virtual const VkPipeline getBasePipelineHandle() const override;

    virtual int32_t getBasePipelineIndex() const override;

    virtual const VkPipeline getPipeline() const override;

    virtual VkTsVertexBufferType getVertexBufferType() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_GRAPHICSPIPELINE_HPP_ */
