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

#include "GraphicsPipeline.hpp"

namespace vkts
{

GraphicsPipeline::GraphicsPipeline(const VkDevice device, const VkPipelineCache pipelineCache, const VkGraphicsPipelineCreateInfo& graphicsPipelineCreateInfo, const VkPipeline pipeline, const VkTsVertexBufferType vertexBufferType) :
    IGraphicsPipeline(), device(device), pipelineCache(pipelineCache), graphicsPipelineCreateInfo(graphicsPipelineCreateInfo), allStagesSerialized(), vertexInputStateSerialized(), inputAssemblyStateSerialized(), tessellationStateSerialized(), viewportStateSerialized(), rasterizationStateSerialized(), multisampleStateSerialized(), depthStencilStateSerialized(), colorBlendStateSerialized(), dynamicStateSerialized(), pipeline(pipeline), vertexBufferType(vertexBufferType)
{
    this->graphicsPipelineCreateInfo.pStages = nullptr;
    this->graphicsPipelineCreateInfo.pVertexInputState = nullptr;
    this->graphicsPipelineCreateInfo.pInputAssemblyState = nullptr;
    this->graphicsPipelineCreateInfo.pTessellationState = nullptr;
    this->graphicsPipelineCreateInfo.pViewportState = nullptr;
    this->graphicsPipelineCreateInfo.pRasterizationState = nullptr;
    this->graphicsPipelineCreateInfo.pMultisampleState = nullptr;
    this->graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
    this->graphicsPipelineCreateInfo.pColorBlendState = nullptr;
    this->graphicsPipelineCreateInfo.pDynamicState = nullptr;

    if (graphicsPipelineCreateInfo.stageCount > 0 && graphicsPipelineCreateInfo.pStages)
    {
        for (uint32_t i = 0; i < graphicsPipelineCreateInfo.stageCount; i++)
        {
            IBinaryBufferSP currentStageSerialized = commonSerializeStructureType(&graphicsPipelineCreateInfo.pStages[i]);

            allStagesSerialized.append(currentStageSerialized);

            if (i == 0)
            {
                this->graphicsPipelineCreateInfo.pStages = (const VkPipelineShaderStageCreateInfo*) currentStageSerialized->getData();
            }
        }
    }

    if (graphicsPipelineCreateInfo.pVertexInputState)
    {
        vertexInputStateSerialized = commonSerializeStructureType(graphicsPipelineCreateInfo.pVertexInputState);

        this->graphicsPipelineCreateInfo.pVertexInputState = (const VkPipelineVertexInputStateCreateInfo*) vertexInputStateSerialized->getData();
    }

    if (graphicsPipelineCreateInfo.pInputAssemblyState)
    {
        inputAssemblyStateSerialized = commonSerializeStructureType(graphicsPipelineCreateInfo.pInputAssemblyState);

        this->graphicsPipelineCreateInfo.pInputAssemblyState = (const VkPipelineInputAssemblyStateCreateInfo*) inputAssemblyStateSerialized->getData();
    }

    if (graphicsPipelineCreateInfo.pTessellationState)
    {
        tessellationStateSerialized = commonSerializeStructureType(graphicsPipelineCreateInfo.pTessellationState);

        this->graphicsPipelineCreateInfo.pTessellationState = (const VkPipelineTessellationStateCreateInfo*) tessellationStateSerialized->getData();
    }

    if (graphicsPipelineCreateInfo.pViewportState)
    {
        viewportStateSerialized = commonSerializeStructureType(graphicsPipelineCreateInfo.pViewportState);

        this->graphicsPipelineCreateInfo.pViewportState = (const VkPipelineViewportStateCreateInfo*) viewportStateSerialized->getData();
    }

    if (graphicsPipelineCreateInfo.pRasterizationState)
    {
        rasterizationStateSerialized = commonSerializeStructureType(graphicsPipelineCreateInfo.pRasterizationState);

        this->graphicsPipelineCreateInfo.pRasterizationState = (const VkPipelineRasterizationStateCreateInfo*) rasterizationStateSerialized->getData();
    }

    if (graphicsPipelineCreateInfo.pMultisampleState)
    {
        multisampleStateSerialized = commonSerializeStructureType(graphicsPipelineCreateInfo.pMultisampleState);

        this->graphicsPipelineCreateInfo.pMultisampleState = (const VkPipelineMultisampleStateCreateInfo*) multisampleStateSerialized->getData();
    }

    if (graphicsPipelineCreateInfo.pDepthStencilState)
    {
        depthStencilStateSerialized = commonSerializeStructureType(graphicsPipelineCreateInfo.pDepthStencilState);

        this->graphicsPipelineCreateInfo.pDepthStencilState = (const VkPipelineDepthStencilStateCreateInfo*) depthStencilStateSerialized->getData();
    }

    if (graphicsPipelineCreateInfo.pColorBlendState)
    {
        colorBlendStateSerialized = commonSerializeStructureType(graphicsPipelineCreateInfo.pColorBlendState);

        this->graphicsPipelineCreateInfo.pColorBlendState = (const VkPipelineColorBlendStateCreateInfo*) colorBlendStateSerialized->getData();
    }

    if (graphicsPipelineCreateInfo.pDynamicState)
    {
        dynamicStateSerialized = commonSerializeStructureType(graphicsPipelineCreateInfo.pDynamicState);

        this->graphicsPipelineCreateInfo.pDynamicState = (const VkPipelineDynamicStateCreateInfo*) dynamicStateSerialized->getData();
    }
}

GraphicsPipeline::~GraphicsPipeline()
{
    destroy();
}

//
// IGraphicsPipeline
//

const VkDevice GraphicsPipeline::getDevice() const
{
    return device;
}

const VkGraphicsPipelineCreateInfo& GraphicsPipeline::getGraphicsPipelineCreateInfo() const
{
    return graphicsPipelineCreateInfo;
}

VkPipelineCreateFlags GraphicsPipeline::getFlags() const
{
    return graphicsPipelineCreateInfo.flags;
}

uint32_t GraphicsPipeline::getStageCount() const
{
    return graphicsPipelineCreateInfo.stageCount;
}

const VkPipelineShaderStageCreateInfo* GraphicsPipeline::getStages() const
{
    return graphicsPipelineCreateInfo.pStages;
}

const VkPipelineVertexInputStateCreateInfo* GraphicsPipeline::getVertexInputState() const
{
    return graphicsPipelineCreateInfo.pVertexInputState;
}

const VkPipelineInputAssemblyStateCreateInfo* GraphicsPipeline::getInputAssemblyState() const
{
    return graphicsPipelineCreateInfo.pInputAssemblyState;
}

const VkPipelineTessellationStateCreateInfo* GraphicsPipeline::getTessellationState() const
{
    return graphicsPipelineCreateInfo.pTessellationState;
}

const VkPipelineViewportStateCreateInfo* GraphicsPipeline::getViewportState() const
{
    return graphicsPipelineCreateInfo.pViewportState;
}

const VkPipelineRasterizationStateCreateInfo* GraphicsPipeline::getRasterizationState() const
{
    return graphicsPipelineCreateInfo.pRasterizationState;
}

const VkPipelineMultisampleStateCreateInfo* GraphicsPipeline::getMultisampleState() const
{
    return graphicsPipelineCreateInfo.pMultisampleState;
}

const VkPipelineDepthStencilStateCreateInfo* GraphicsPipeline::getDepthStencilState() const
{
    return graphicsPipelineCreateInfo.pDepthStencilState;
}

const VkPipelineColorBlendStateCreateInfo* GraphicsPipeline::getColorBlendState() const
{
    return graphicsPipelineCreateInfo.pColorBlendState;
}

const VkPipelineDynamicStateCreateInfo* GraphicsPipeline::getDynamicState() const
{
    return graphicsPipelineCreateInfo.pDynamicState;
}

const VkPipelineLayout GraphicsPipeline::getLayout() const
{
    return graphicsPipelineCreateInfo.layout;
}

const VkRenderPass GraphicsPipeline::getRenderPass() const
{
    return graphicsPipelineCreateInfo.renderPass;
}

uint32_t GraphicsPipeline::getSubpass() const
{
    return graphicsPipelineCreateInfo.subpass;
}

const VkPipeline GraphicsPipeline::getBasePipelineHandle() const
{
    return graphicsPipelineCreateInfo.basePipelineHandle;
}

int32_t GraphicsPipeline::getBasePipelineIndex() const
{
    return graphicsPipelineCreateInfo.basePipelineIndex;
}

const VkPipeline GraphicsPipeline::getPipeline() const
{
    return pipeline;
}

VkTsVertexBufferType GraphicsPipeline::getVertexBufferType() const
{
    return vertexBufferType ;
}

//
// IDestroyable
//

void GraphicsPipeline::destroy()
{
    if (pipeline)
    {
        vkDestroyPipeline(device, pipeline, nullptr);

        pipeline = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
