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

#include "PipelineLayout.hpp"

namespace vkts
{

PipelineLayout::PipelineLayout(const VkDevice device, const VkPipelineLayoutCreateFlags flags, const uint32_t setLayoutCount, const VkDescriptorSetLayout* setLayouts, const uint32_t pushConstantRangeCount, const VkPushConstantRange* pushConstantRanges, const VkPipelineLayout pipelineLayout) :
    IPipelineLayout(), device(device), pipelineLayoutCreateInfo{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, nullptr, flags, setLayoutCount, nullptr, pushConstantRangeCount, nullptr}, allDescriptorSetLayouts(), pipelineLayout(pipelineLayout)
{
    for (uint32_t i = 0; i < setLayoutCount; i++)
    {
        allDescriptorSetLayouts.push_back(setLayouts[i]);
    }

    if (setLayoutCount > 0)
    {
        pipelineLayoutCreateInfo.pSetLayouts = &(allDescriptorSetLayouts[0]);
    }

    for (uint32_t i = 0; i < pushConstantRangeCount; i++)
    {
        allPushConstantRanges.push_back(pushConstantRanges[i]);
    }

    if (pushConstantRangeCount > 0)
    {
        pipelineLayoutCreateInfo.pPushConstantRanges = &(allPushConstantRanges[0]);
    }
}

PipelineLayout::~PipelineLayout()
{
    destroy();
}

//
// IPipelineLayout
//

const VkDevice PipelineLayout::getDevice() const
{
    return device;
}

const VkPipelineLayoutCreateInfo& PipelineLayout::getPipelineLayoutCreateInfo() const
{
    return pipelineLayoutCreateInfo;
}

VkPipelineLayoutCreateFlags PipelineLayout::getFlags() const
{
    return pipelineLayoutCreateInfo.flags;
}

uint32_t PipelineLayout::getSetLayoutCount() const
{
    return pipelineLayoutCreateInfo.setLayoutCount;
}

const VkDescriptorSetLayout* PipelineLayout::getSetLayouts() const
{
    return pipelineLayoutCreateInfo.pSetLayouts;
}

uint32_t PipelineLayout::getPushConstantRangeCount() const
{
    return pipelineLayoutCreateInfo.pushConstantRangeCount;
}

const VkPushConstantRange* PipelineLayout::getPushConstantRanges() const
{
    return pipelineLayoutCreateInfo.pPushConstantRanges;
}

const VkPipelineLayout PipelineLayout::getPipelineLayout() const
{
    return pipelineLayout;
}

//
// IDestroyable
//

void PipelineLayout::destroy()
{
    if (pipelineLayout)
    {
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

        pipelineLayout = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
