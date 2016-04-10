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

#ifndef VKTS_COMMANDPOOL_HPP_
#define VKTS_COMMANDPOOL_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class CommandPool: public ICommandPool
{

private:

    const VkDevice device;

    const VkCommandPoolCreateInfo commandPoolCreateInfo;

    VkCommandPool commandPool;

public:

    CommandPool() = delete;
    CommandPool(const VkDevice device, const VkCommandPoolCreateFlags flags, const uint32_t queueFamilyIndex, const VkCommandPool commandPool);
    CommandPool(const CommandPool& other) = delete;
    CommandPool(CommandPool&& other) = delete;
    virtual ~CommandPool();

    CommandPool& operator =(const CommandPool& other) = delete;
    CommandPool& operator =(CommandPool && other) = delete;

    //
    // ICommandPool
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkCommandPoolCreateInfo& getCommandPoolCreateInfo() const override;

    virtual VkCommandPoolCreateFlags getFlags() const override;

    virtual uint32_t getQueueFamilyIndex() const override;

    virtual const VkCommandPool getCmdPool() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_COMMANDPOOL_HPP_ */
