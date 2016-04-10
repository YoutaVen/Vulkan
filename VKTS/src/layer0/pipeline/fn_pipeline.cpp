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

#include <vkts/vkts.hpp>

#include "PipelineCache.hpp"
#include "PipelineLayout.hpp"
#include "ComputePipeline.hpp"
#include "GraphicsPipeline.hpp"

namespace vkts
{

IPipelineCacheSP VKTS_APIENTRY pipelineCreateCache(const VkDevice device, const VkPipelineCacheCreateFlags flags, const size_t initialDataSize, const void* initialData)
{
    if (!device)
    {
        return IPipelineCacheSP();
    }

    VkResult result;

    VkPipelineCacheCreateInfo pipelineCacheCreateInfo;

    memset(&pipelineCacheCreateInfo, 0, sizeof(VkPipelineCacheCreateInfo));

    pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

    pipelineCacheCreateInfo.flags = flags;
    pipelineCacheCreateInfo.initialDataSize = initialDataSize;
    pipelineCacheCreateInfo.pInitialData = initialData;

    VkPipelineCache pipelineCache;

    result = vkCreatePipelineCache(device, &pipelineCacheCreateInfo, nullptr, &pipelineCache);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "Pipeline: Could not create pipeline cache.");

        return IPipelineCacheSP();
    }

    auto newInstance = new PipelineCache(device, flags, initialDataSize, initialData, pipelineCache);

    if (!newInstance)
    {
        vkDestroyPipelineCache(device, pipelineCache, nullptr);

        return IPipelineCacheSP();
    }

    return IPipelineCacheSP(newInstance);
}

IPipelineLayoutSP VKTS_APIENTRY pipelineCreateLayout(const VkDevice device, const VkPipelineLayoutCreateFlags flags, const uint32_t setLayoutCount, const VkDescriptorSetLayout* setLayouts, const uint32_t pushConstantRangeCount, const VkPushConstantRange* pushConstantRanges)
{
    if (!device)
    {
        return IPipelineLayoutSP();
    }

    VkResult result;

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;

    memset(&pipelineLayoutCreateInfo, 0, sizeof(VkPipelineLayoutCreateInfo));

    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    pipelineLayoutCreateInfo.flags = flags;
    pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount;
    pipelineLayoutCreateInfo.pSetLayouts = setLayouts;
    pipelineLayoutCreateInfo.pushConstantRangeCount = pushConstantRangeCount;
    pipelineLayoutCreateInfo.pPushConstantRanges = pushConstantRanges;

    VkPipelineLayout pipelineLayout;

    result = vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "Pipeline: Could not create pipeline layout.");

        return IPipelineLayoutSP();
    }

    auto newInstance = new PipelineLayout(device, flags, setLayoutCount, setLayouts, pushConstantRangeCount, pushConstantRanges, pipelineLayout);

    if (!newInstance)
    {
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

        return IPipelineLayoutSP();
    }

    return IPipelineLayoutSP(newInstance);
}

IGraphicsPipelineSP VKTS_APIENTRY pipelineCreateGraphics(const VkDevice device, const VkPipelineCache pipelineCache, const VkGraphicsPipelineCreateInfo& graphicsPipelineCreateInfo, const VkTsVertexBufferType vertexBufferType)
{
    if (!device)
    {
        return IGraphicsPipelineSP();
    }

    VkResult result;

    VkPipeline pipeline;

    result = vkCreateGraphicsPipelines(device, pipelineCache, 1, &graphicsPipelineCreateInfo, nullptr, &pipeline);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "GraphicsPipeline: Could not create graphics pipeline.");

        return IGraphicsPipelineSP();
    }

    auto newInstance = new GraphicsPipeline(device, pipelineCache, graphicsPipelineCreateInfo, pipeline, vertexBufferType);

    if (!newInstance)
    {
        vkDestroyPipeline(device, pipeline, nullptr);

        return IGraphicsPipelineSP();
    }

    return IGraphicsPipelineSP(newInstance);
}

IComputePipelineSP VKTS_APIENTRY pipelineCreateCompute(const VkDevice device, const VkPipelineCache pipelineCache, const VkComputePipelineCreateInfo& computePipelineCreateInfo)
{
    if (!device)
    {
        return IComputePipelineSP();
    }

    VkResult result;

    VkPipeline pipeline;

    result = vkCreateComputePipelines(device, pipelineCache, 1, &computePipelineCreateInfo, nullptr, &pipeline);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "ComputePipeline: Could not create compute pipeline.");

        return IComputePipelineSP();
    }

    auto newInstance = new ComputePipeline(device, pipelineCache, computePipelineCreateInfo, pipeline);

    if (!newInstance)
    {
        vkDestroyPipeline(device, pipeline, nullptr);

        return IComputePipelineSP();
    }

    return IComputePipelineSP(newInstance);
}

}
