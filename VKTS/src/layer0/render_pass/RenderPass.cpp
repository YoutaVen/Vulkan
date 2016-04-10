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

#include "RenderPass.hpp"

namespace vkts
{

RenderPass::RenderPass(const VkDevice device, const VkRenderPassCreateFlags flags, const uint32_t attachmentCount, const VkAttachmentDescription* attachments, const uint32_t subpassCount, const VkSubpassDescription* subpasses, const uint32_t dependencyCount, const VkSubpassDependency* dependencies, const VkRenderPass renderPass) :
    IRenderPass(), device(device), renderPassCreateInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, nullptr, flags, attachmentCount, nullptr, subpassCount, nullptr, dependencyCount, nullptr}, renderPass(renderPass)
{
    for (uint32_t i = 0; i < attachmentCount; i++)
    {
        allAttachments.push_back(attachments[i]);
    }

    if (allAttachments.size() > 0)
    {
        renderPassCreateInfo.pAttachments = &allAttachments[0];
    }

    for (uint32_t i = 0; i < subpassCount; i++)
    {
        // No deep copy.
        allSubpasses.push_back(subpasses[i]);
    }

    if (allSubpasses.size() > 0)
    {
        renderPassCreateInfo.pSubpasses = &allSubpasses[0];
    }

    for (uint32_t i = 0; i < dependencyCount; i++)
    {
        allDependencies.push_back(dependencies[i]);
    }

    if (allDependencies.size() > 0)
    {
        renderPassCreateInfo.pDependencies = &allDependencies[0];
    }
}

RenderPass::~RenderPass()
{
    destroy();
}

//
// IRenderPass
//

const VkDevice RenderPass::getDevice() const
{
    return device;
}

const VkRenderPassCreateInfo& RenderPass::getRenderPassCreateInfo() const
{
    return renderPassCreateInfo;
}

VkRenderPassCreateFlags RenderPass::getFlags() const
{
    return renderPassCreateInfo.flags;
}

uint32_t RenderPass::getAttachmentCount() const
{
    return renderPassCreateInfo.attachmentCount;
}

const VkAttachmentDescription* RenderPass::getAttachments() const
{
    return renderPassCreateInfo.pAttachments;
}

uint32_t RenderPass::getSubpassCount() const
{
    return renderPassCreateInfo.subpassCount;
}

const VkSubpassDescription* RenderPass::getSubpasses() const
{
    return renderPassCreateInfo.pSubpasses;
}

uint32_t RenderPass::getDependencyCount() const
{
    return renderPassCreateInfo.dependencyCount;
}

const VkSubpassDependency* RenderPass::getDependencies() const
{
    return renderPassCreateInfo.pDependencies;
}

const VkRenderPass RenderPass::getRenderPass() const
{
    return renderPass;
}

//
// IDestroyable
//

void RenderPass::destroy()
{
    if (renderPass)
    {
        vkDestroyRenderPass(device, renderPass, nullptr);

        renderPass = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
