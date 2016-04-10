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

#ifndef VKTS_COMMANDBUFFERS_HPP_
#define VKTS_COMMANDBUFFERS_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class CommandBuffers: public ICommandBuffers
{

private:

    const VkDevice device;

    const VkCommandBufferAllocateInfo commandBufferAllocateInfo;

    std::vector<VkCommandBuffer> allCommandBuffers;

    VkCommandBufferResetFlags commandBufferResetFlags;

public:

    CommandBuffers() = delete;
    CommandBuffers(const VkDevice device, const VkCommandPool commandPool, const VkCommandBufferLevel level, const uint32_t commandBufferCount, const VkCommandBuffer* commandBuffers);
    CommandBuffers(const CommandBuffers& other) = delete;
    CommandBuffers(CommandBuffers&& other) = delete;
    virtual ~CommandBuffers();

    CommandBuffers& operator =(const CommandBuffers& other) = delete;

    CommandBuffers& operator =(CommandBuffers && other) = delete;

    //
    // ICommandBuffer
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkCommandBufferAllocateInfo& getCommadBufferAllocateInfo() const override;

    virtual const VkCommandPool getCommandPool() const override;

    virtual VkCommandBufferLevel getLevel() const override;

    virtual uint32_t getCommandBufferCount() const override;

    virtual const VkCommandBuffer getCommandBuffer(const uint32_t bufferIndex = 0) const override;

    virtual const VkCommandBuffer* getCommandBuffers() const override;

    virtual VkResult beginCommandBuffer(const VkCommandBufferBeginInfo* cmdBufferBeginInfo, const uint32_t bufferIndex = 0) const override;

    virtual VkResult beginCommandBuffer(const VkCommandBufferUsageFlags flags, const VkRenderPass renderPass, const uint32_t subpass, const VkFramebuffer framebuffer, const VkBool32 occlusionQueryEnable, const VkQueryControlFlags queryFlags, const VkQueryPipelineStatisticFlags pipelineStatistics, const uint32_t bufferIndex = 0) const override;

    virtual void cmdPipelineBarrier(const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkDependencyFlags dependencyFlags, const uint32_t memoryBarrierCount, const VkMemoryBarrier* memoryBarriers, const uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* bufferMemoryBarriers, const uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* imageMemoryBarriers, const uint32_t bufferIndex = 0) const override;

    virtual void cmdClearColorImage(const VkImage image, const VkImageLayout imageLayout, const VkClearColorValue* color, const uint32_t rangeCount, const VkImageSubresourceRange* ranges, const uint32_t bufferIndex = 0) const override;

    virtual void cmdClearDepthStencilImage(const VkImage image, const VkImageLayout imageLayout, const VkClearDepthStencilValue* depthStencil, const uint32_t rangeCount, const VkImageSubresourceRange* ranges, const uint32_t bufferIndex = 0) const override;

    virtual void cmdBeginRenderPass(const VkRenderPassBeginInfo* renderPassBeginInfo, const VkSubpassContents contents, const uint32_t bufferIndex = 0) const override;

    virtual void cmdEndRenderPass(const uint32_t bufferIndex = 0) const override;

    virtual VkResult endCommandBuffer(const uint32_t bufferIndex = 0) const override;

	virtual VkCommandBufferResetFlags getCommandBufferResetFlags() const override;

	virtual void setCommandBufferResetFlags(const VkCommandBufferResetFlags commandBufferResetFlags) override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

    //
    // IResetable
    //

    virtual VkResult reset() override;
};

} /* namespace vkts */

#endif /* VKTS_COMMANDBUFFERS_HPP_ */
