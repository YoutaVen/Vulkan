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

#include "CommandBuffers.hpp"

namespace vkts
{

CommandBuffers::CommandBuffers(const VkDevice device, const VkCommandPool commandPool, const VkCommandBufferLevel level, const uint32_t commandBufferCount, const VkCommandBuffer* commandBuffers) :
    ICommandBuffers(), device(device), commandBufferAllocateInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, nullptr, commandPool, level, commandBufferCount}, allCommandBuffers(), commandBufferResetFlags()
{
    for (uint32_t i = 0; i < commandBufferCount; i++)
    {
        allCommandBuffers.push_back(commandBuffers[i]);
    }
}

CommandBuffers::~CommandBuffers()
{
    destroy();
}

//
// ICommandBuffer
//

const VkCommandBuffer CommandBuffers::getCommandBuffer(const uint32_t bufferIndex) const
{
    return allCommandBuffers[bufferIndex];
}

const VkCommandBuffer* CommandBuffers::getCommandBuffers() const
{
    return &allCommandBuffers[0];
}

const VkCommandBufferAllocateInfo& CommandBuffers::getCommadBufferAllocateInfo() const
{
    return commandBufferAllocateInfo;
}

const VkCommandPool CommandBuffers::getCommandPool() const
{
    return commandBufferAllocateInfo.commandPool;
}

VkCommandBufferLevel CommandBuffers::getLevel() const
{
    return commandBufferAllocateInfo.level;
}

uint32_t CommandBuffers::getCommandBufferCount() const
{
    return commandBufferAllocateInfo.commandBufferCount;
}

const VkDevice CommandBuffers::getDevice() const
{
    return device;
}

VkResult CommandBuffers::beginCommandBuffer(const VkCommandBufferBeginInfo* cmdBufferBeginInfo, const uint32_t bufferIndex) const
{
    return vkBeginCommandBuffer(allCommandBuffers[bufferIndex], cmdBufferBeginInfo);
}

VkResult CommandBuffers::beginCommandBuffer(const VkCommandBufferUsageFlags flags, const VkRenderPass renderPass, const uint32_t subpass, const VkFramebuffer framebuffer, const VkBool32 occlusionQueryEnable, const VkQueryControlFlags queryFlags, const VkQueryPipelineStatisticFlags pipelineStatistics, const uint32_t bufferIndex) const
{
	VkCommandBufferInheritanceInfo commandBufferInheritanceInfo;

	memset(&commandBufferInheritanceInfo, 0, sizeof(VkCommandBufferInheritanceInfo));

	commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;

	commandBufferInheritanceInfo.renderPass = renderPass;
	commandBufferInheritanceInfo.subpass = subpass;
	commandBufferInheritanceInfo.framebuffer = framebuffer;
	commandBufferInheritanceInfo.occlusionQueryEnable = occlusionQueryEnable;
	commandBufferInheritanceInfo.queryFlags = queryFlags;
	commandBufferInheritanceInfo.pipelineStatistics = pipelineStatistics;


    VkCommandBufferBeginInfo commandBufferBeginInfo;

    memset(&commandBufferBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));

    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    commandBufferBeginInfo.flags = flags;
    commandBufferBeginInfo.pInheritanceInfo = &commandBufferInheritanceInfo;

    return vkBeginCommandBuffer(allCommandBuffers[bufferIndex], &commandBufferBeginInfo);
}

void CommandBuffers::cmdPipelineBarrier(const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkDependencyFlags dependencyFlags, const uint32_t memoryBarrierCount, const VkMemoryBarrier* memoryBarriers, const uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* bufferMemoryBarriers, const uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* imageMemoryBarriers, const uint32_t bufferIndex) const
{
    vkCmdPipelineBarrier(allCommandBuffers[bufferIndex], srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, memoryBarriers, bufferMemoryBarrierCount, bufferMemoryBarriers, imageMemoryBarrierCount, imageMemoryBarriers);
}

void CommandBuffers::cmdClearColorImage(const VkImage image, const VkImageLayout imageLayout, const VkClearColorValue* color, const uint32_t rangeCount, const VkImageSubresourceRange* ranges, const uint32_t bufferIndex) const
{
    vkCmdClearColorImage(allCommandBuffers[bufferIndex], image, imageLayout, color, rangeCount, ranges);
}

void CommandBuffers::cmdClearDepthStencilImage(const VkImage image, const VkImageLayout imageLayout, const VkClearDepthStencilValue* depthStencil, const uint32_t rangeCount, const VkImageSubresourceRange* ranges, const uint32_t bufferIndex) const
{
    vkCmdClearDepthStencilImage(allCommandBuffers[bufferIndex], image, imageLayout, depthStencil, rangeCount, ranges);
}

void CommandBuffers::cmdBeginRenderPass(const VkRenderPassBeginInfo* renderPassBeginInfo, const VkSubpassContents contents, const uint32_t bufferIndex) const
{
    vkCmdBeginRenderPass(allCommandBuffers[bufferIndex], renderPassBeginInfo, contents);
}

void CommandBuffers::cmdEndRenderPass(const uint32_t bufferIndex) const
{
    vkCmdEndRenderPass(allCommandBuffers[bufferIndex]);
}

VkResult CommandBuffers::endCommandBuffer(const uint32_t bufferIndex) const
{
    return vkEndCommandBuffer(allCommandBuffers[bufferIndex]);
}

//
// IDestroyable
//

void CommandBuffers::destroy()
{
    if (allCommandBuffers.size() > 0)
    {
        vkFreeCommandBuffers(device, commandBufferAllocateInfo.commandPool, (uint32_t)allCommandBuffers.size(), &allCommandBuffers[0]);

        allCommandBuffers.clear();
    }
}

VkCommandBufferResetFlags CommandBuffers::getCommandBufferResetFlags() const
{
	return commandBufferResetFlags;
}

void CommandBuffers::setCommandBufferResetFlags(VkCommandBufferResetFlags commandBufferResetFlags)
{
	this->commandBufferResetFlags = commandBufferResetFlags;
}

//
// IResetable
//

VkResult CommandBuffers::reset()
{
	VkResult result = VK_SUCCESS;

	for (size_t i = 0; i < allCommandBuffers.size(); i++)
	{
		result = vkResetCommandBuffer(allCommandBuffers[i], commandBufferResetFlags);

		if (result != VK_SUCCESS)
		{
			break;
		}
	}

	return result;
}

} /* namespace vkts */
