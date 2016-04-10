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

#include "Framebuffer.hpp"

namespace vkts
{

Framebuffer::Framebuffer(const VkDevice device, const VkFramebufferCreateFlags flags, const VkRenderPass renderPass, const uint32_t attachmentCount, const VkImageView* attachments, const uint32_t width, const uint32_t height, const uint32_t layers, const VkFramebuffer framebuffer) :
    IFramebuffer(), device(device), framebufferCreateInfo{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, nullptr, flags, renderPass, attachmentCount, nullptr, width, height, layers}, allAttachmentBindInfos(), framebuffer(framebuffer)
{
    for (uint32_t i = 0; i < attachmentCount; i++)
    {
        allAttachmentBindInfos.push_back(attachments[i]);
    }

    if (attachmentCount > 0)
    {
        framebufferCreateInfo.pAttachments = &(allAttachmentBindInfos[0]);
    }
}

Framebuffer::~Framebuffer()
{
    destroy();
}

//
// IFramebuffer
//

const VkDevice Framebuffer::getDevice() const
{
    return device;
}

const VkFramebufferCreateInfo& Framebuffer::getFramebufferCreateInfo() const
{
    return framebufferCreateInfo;
}

VkFramebufferCreateFlags Framebuffer::getFlags() const
{
    return framebufferCreateInfo.flags;
}

const VkRenderPass Framebuffer::getRenderPass() const
{
    return framebufferCreateInfo.renderPass;
}

uint32_t Framebuffer::getAttachmentCount() const
{
    return framebufferCreateInfo.attachmentCount;
}

const VkImageView* Framebuffer::getAttachments() const
{
    return framebufferCreateInfo.pAttachments;
}

uint32_t Framebuffer::getWidth() const
{
    return framebufferCreateInfo.width;
}

uint32_t Framebuffer::getHeight() const
{
    return framebufferCreateInfo.height;
}

uint32_t Framebuffer::getLayers() const
{
    return framebufferCreateInfo.layers;
}

const VkFramebuffer Framebuffer::getFramebuffer() const
{
    return framebuffer;
}

//
// IDestroyable
//

void Framebuffer::destroy()
{
    if (framebuffer)
    {
        vkDestroyFramebuffer(device, framebuffer, nullptr);

        framebuffer = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
