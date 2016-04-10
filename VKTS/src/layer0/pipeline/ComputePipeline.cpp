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

#include "ComputePipeline.hpp"

namespace vkts
{

ComputePipeline::ComputePipeline(const VkDevice device, const VkPipelineCache pipelineCache, const VkComputePipelineCreateInfo& computePipelineCreateInfo, const VkPipeline pipeline) :
    IComputePipeline(), device(device), pipelineCache(pipelineCache), computePipelineCreateInfo(computePipelineCreateInfo), pipeline(pipeline)
{
}

ComputePipeline::~ComputePipeline()
{
    destroy();
}

//
// IComputePipeline
//

const VkDevice ComputePipeline::getDevice() const
{
    return device;
}

const VkComputePipelineCreateInfo& ComputePipeline::getComputePipelineCreateInfo() const
{
    return computePipelineCreateInfo;
}

VkPipelineCreateFlags ComputePipeline::getFlags() const
{
    return computePipelineCreateInfo.flags;
}

const VkPipelineShaderStageCreateInfo& ComputePipeline::getStage() const
{
    return computePipelineCreateInfo.stage;
}

const VkPipelineLayout ComputePipeline::getLayout() const
{
    return computePipelineCreateInfo.layout;
}

const VkPipeline ComputePipeline::getBasePipelineHandle() const
{
    return computePipelineCreateInfo.basePipelineHandle;
}

int32_t ComputePipeline::getBasePipelineIndex() const
{
    return computePipelineCreateInfo.basePipelineIndex;
}

const VkPipeline ComputePipeline::getPipeline() const
{
    return pipeline;
}

//
// IDestroyable
//

void ComputePipeline::destroy()
{
    if (pipeline)
    {
        vkDestroyPipeline(device, pipeline, nullptr);

        pipeline = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
