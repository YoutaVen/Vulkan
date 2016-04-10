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

#ifndef VKTS_FRAMEBUFFER_HPP_
#define VKTS_FRAMEBUFFER_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Framebuffer: public IFramebuffer
{

private:

    const VkDevice device;

    VkFramebufferCreateInfo framebufferCreateInfo;

    std::vector<VkImageView> allAttachmentBindInfos;

    VkFramebuffer framebuffer;

public:

    Framebuffer() = delete;
    Framebuffer(const VkDevice device, const VkFramebufferCreateFlags flags, const VkRenderPass renderPass, const uint32_t attachmentCount, const VkImageView* attachments, const uint32_t width, const uint32_t height, const uint32_t layers, const VkFramebuffer framebuffer);
    Framebuffer(const Framebuffer& other) = delete;
    Framebuffer(Framebuffer&& other) = delete;
    virtual ~Framebuffer();

    Framebuffer& operator =(const Framebuffer& other) = delete;

    Framebuffer& operator =(Framebuffer && other) = delete;

    //
    // IFramebuffer
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkFramebufferCreateInfo& getFramebufferCreateInfo() const override;

    virtual VkFramebufferCreateFlags getFlags() const override;

    virtual const VkRenderPass getRenderPass() const override;

    virtual uint32_t getAttachmentCount() const override;

    virtual const VkImageView* getAttachments() const override;

    virtual uint32_t getWidth() const override;

    virtual uint32_t getHeight() const override;

    virtual uint32_t getLayers() const override;

    virtual const VkFramebuffer getFramebuffer() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_FRAMEBUFFER_HPP_ */
