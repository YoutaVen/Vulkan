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

#ifndef VKTS_RENDERPASS_HPP_
#define VKTS_RENDERPASS_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class RenderPass: public IRenderPass
{

private:

    const VkDevice device;

    VkRenderPassCreateInfo renderPassCreateInfo;

    std::vector<VkAttachmentDescription> allAttachments;
    std::vector<VkSubpassDescription> allSubpasses;
    std::vector<VkSubpassDependency> allDependencies;

    VkRenderPass renderPass;

public:

    RenderPass() = delete;
    RenderPass(const VkDevice device, const VkRenderPassCreateFlags flags, const uint32_t attachmentCount, const VkAttachmentDescription* attachments, const uint32_t subpassCount, const VkSubpassDescription* subpasses, const uint32_t dependencyCount, const VkSubpassDependency* dependencies, const VkRenderPass renderPass);
    RenderPass(const RenderPass& other) = delete;
    RenderPass(RenderPass&& other) = delete;
    virtual ~RenderPass();

    RenderPass& operator =(const RenderPass& other) = delete;

    RenderPass& operator =(RenderPass && other) = delete;

    //
    // IRenderPass
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkRenderPassCreateInfo& getRenderPassCreateInfo() const override;

    virtual VkRenderPassCreateFlags getFlags() const override;

    virtual uint32_t getAttachmentCount() const override;

    virtual const VkAttachmentDescription* getAttachments() const override;

    virtual uint32_t getSubpassCount() const override;

    virtual const VkSubpassDescription* getSubpasses() const override;

    virtual uint32_t getDependencyCount() const override;

    virtual const VkSubpassDependency* getDependencies() const override;

    virtual const VkRenderPass getRenderPass() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_RENDERPASS_HPP_ */
