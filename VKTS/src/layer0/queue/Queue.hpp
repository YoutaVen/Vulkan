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

#ifndef VKTS_QUEUE_HPP_
#define VKTS_QUEUE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Queue: public IQueue
{

private:

    const VkDevice device;

    const uint32_t queueFamilyIndex;

    const uint32_t queueIndex;

    const VkQueue queue;

public:

    Queue() = delete;
    Queue(const VkDevice device, const uint32_t queueFamilyIndex, const uint32_t queueIndex, const VkQueue queue);
    Queue(const Queue& other) = delete;
    Queue(Queue&& other) = delete;
    virtual ~Queue();

    Queue& operator =(const Queue& other) = delete;

    Queue& operator =(Queue && other) = delete;

    //
    // IQueue
    //

    virtual VkResult submit(const uint32_t submitCount, const VkSubmitInfo* submits, const VkFence fence) const override;

    virtual VkResult waitIdle() const override;

    virtual VkResult bindSparse(const uint32_t bindInfoCount, const VkBindSparseInfo* bindInfo, const VkFence fence) const override;

    virtual const VkDevice getDevice() const override;

    virtual uint32_t getQueueFamilyIndex() const override;

    virtual uint32_t getQueueIndex() const override;

    virtual const VkQueue getQueue() const override;

};

} /* namespace vkts */

#endif /* VKTS_QUEUE_HPP_ */
