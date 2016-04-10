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

#ifndef VKTS_FENCE_HPP_
#define VKTS_FENCE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Fence: public IFence
{

private:

    const VkDevice device;

    const VkFenceCreateInfo fenceCreateInfo;

    VkFence fence;

public:

    Fence() = delete;
    Fence(const VkDevice device, const VkFenceCreateFlags flags, const VkFence fence);
    Fence(const Fence& other) = delete;
    Fence(Fence&& other) = delete;
    virtual ~Fence();

    Fence& operator =(const Fence& other) = delete;
    Fence& operator =(Fence && other) = delete;

    //
    // IFence
    //

    virtual VkResult resetFence() const override;

    virtual VkResult getStatus() const override;

    virtual VkResult waitForFence(const uint64_t timeout) const override;

    virtual const VkDevice getDevice() const override;

    virtual const VkFenceCreateInfo& getFenceCreateInfo() const override;

    virtual VkFenceCreateFlags getFlags() const override;

    virtual const VkFence getFence() const override;

    //
    // IResetable
    //

    virtual VkResult reset() override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_FENCE_HPP_ */
