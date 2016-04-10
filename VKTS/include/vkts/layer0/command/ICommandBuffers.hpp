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

#ifndef VKTS_ICOMMANDBUFFERS_HPP_
#define VKTS_ICOMMANDBUFFERS_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class ICommandBuffers: public IDestroyable, public IResetable
{

public:

    ICommandBuffers() :
        IDestroyable(), IResetable()
    {
    }

    virtual ~ICommandBuffers()
    {
    }

    virtual const VkDevice getDevice() const = 0;

    virtual const VkCommandBufferAllocateInfo& getCommadBufferAllocateInfo() const = 0;

    virtual const VkCommandPool getCommandPool() const = 0;

    virtual VkCommandBufferLevel getLevel() const = 0;

    virtual uint32_t getCommandBufferCount() const = 0;

    virtual const VkCommandBuffer getCommandBuffer(const uint32_t bufferIndex = 0) const = 0;

    virtual const VkCommandBuffer* getCommandBuffers() const = 0;

    virtual VkResult beginCommandBuffer(const VkCommandBufferBeginInfo* cmdBufferBeginInfo, const uint32_t bufferIndex = 0) const = 0;

    virtual VkResult beginCommandBuffer(const VkCommandBufferUsageFlags flags, const VkRenderPass renderPass, const uint32_t subpass, const VkFramebuffer framebuffer, const VkBool32 occlusionQueryEnable, const VkQueryControlFlags queryFlags, const VkQueryPipelineStatisticFlags pipelineStatistics, const uint32_t bufferIndex = 0) const = 0;

    virtual void cmdPipelineBarrier(const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkDependencyFlags dependencyFlags, const uint32_t memoryBarrierCount, const VkMemoryBarrier* memoryBarriers, const uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* bufferMemoryBarriers, const uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* imageMemoryBarriers, const uint32_t bufferIndex = 0) const = 0;

    virtual void cmdClearColorImage(const VkImage image, const VkImageLayout imageLayout, const VkClearColorValue* color, const uint32_t rangeCount, const VkImageSubresourceRange* ranges, const uint32_t bufferIndex = 0) const = 0;

    virtual void cmdClearDepthStencilImage(const VkImage image, const VkImageLayout imageLayout, const VkClearDepthStencilValue* depthStencil, const uint32_t rangeCount, const VkImageSubresourceRange* ranges, const uint32_t bufferIndex = 0) const = 0;

    virtual void cmdBeginRenderPass(const VkRenderPassBeginInfo* renderPassBeginInfo, const VkSubpassContents contents, const uint32_t bufferIndex = 0) const = 0;

    virtual void cmdEndRenderPass(const uint32_t bufferIndex = 0) const = 0;

    virtual VkResult endCommandBuffer(const uint32_t bufferIndex = 0) const = 0;

	virtual VkCommandBufferResetFlags getCommandBufferResetFlags() const = 0;

	virtual void setCommandBufferResetFlags(const VkCommandBufferResetFlags commandBufferResetFlags) = 0;

};

typedef std::shared_ptr<ICommandBuffers> ICommandBuffersSP;

} /* namespace vkts */

#endif /* VKTS_iCOMMANDBUFFERS_HPP_ */
