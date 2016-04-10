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

#ifndef VKTS_PIPELINELAYOUT_HPP_
#define VKTS_PIPELINELAYOUT_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class PipelineLayout: public IPipelineLayout
{

private:

    const VkDevice device;

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;

    std::vector<VkDescriptorSetLayout> allDescriptorSetLayouts;

    std::vector<VkPushConstantRange> allPushConstantRanges;

    VkPipelineLayout pipelineLayout;

public:

    PipelineLayout() = delete;
    PipelineLayout(const VkDevice device, const VkPipelineLayoutCreateFlags flags, const uint32_t setLayoutCount, const VkDescriptorSetLayout* setLayouts, const uint32_t pushConstantRangeCount, const VkPushConstantRange* pushConstantRanges, const VkPipelineLayout pipelineLayout);
    PipelineLayout(const PipelineLayout& other) = delete;
    PipelineLayout(PipelineLayout&& other) = delete;
    virtual ~PipelineLayout();

    PipelineLayout& operator =(const PipelineLayout& other) = delete;

    PipelineLayout& operator =(PipelineLayout && other) = delete;

    //
    // IPipelineLayout
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkPipelineLayoutCreateInfo& getPipelineLayoutCreateInfo() const override;

    virtual VkPipelineLayoutCreateFlags getFlags() const override;

    virtual uint32_t getSetLayoutCount() const override;

    virtual const VkDescriptorSetLayout* getSetLayouts() const override;

    virtual uint32_t getPushConstantRangeCount() const override;

    virtual const VkPushConstantRange* getPushConstantRanges() const override;

    virtual const VkPipelineLayout getPipelineLayout() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_PIPELINELAYOUT_HPP_ */
