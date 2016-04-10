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

#include "RenderPass.hpp"

namespace vkts
{

IRenderPassSP VKTS_APIENTRY renderPassCreate(const VkDevice device, const VkRenderPassCreateFlags flags, const uint32_t attachmentCount, const VkAttachmentDescription* attachments, const uint32_t subpassCount, const VkSubpassDescription* subpasses, const uint32_t dependencyCount, const VkSubpassDependency* dependencies)
{
    if (!device)
    {
        return IRenderPassSP();
    }

    VkResult result;

    VkRenderPassCreateInfo renderPassCreateInfo;

    memset(&renderPassCreateInfo, 0, sizeof(VkRenderPassCreateInfo));

    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    renderPassCreateInfo.flags = flags;
    renderPassCreateInfo.attachmentCount = attachmentCount;
    renderPassCreateInfo.pAttachments = attachments;
    renderPassCreateInfo.subpassCount = subpassCount;
    renderPassCreateInfo.pSubpasses = subpasses;
    renderPassCreateInfo.dependencyCount = dependencyCount;
    renderPassCreateInfo.pDependencies = dependencies;

    VkRenderPass renderPass;

    result = vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "RenderPass: Could not create render pass.");

        return IRenderPassSP();
    }

    auto newInstance = new RenderPass(device, flags, attachmentCount, attachments, subpassCount, subpasses, dependencyCount, dependencies, renderPass);

    if (!newInstance)
    {
        vkDestroyRenderPass(device, renderPass, nullptr);

        return IRenderPassSP();
    }

    return IRenderPassSP(newInstance);
}

}
