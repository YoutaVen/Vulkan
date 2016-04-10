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

#ifndef VKTS_BUFFERVIEW_HPP_
#define VKTS_BUFFERVIEW_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class BufferView: public IBufferView
{

private:

    const VkDevice device;

    const VkBufferViewCreateInfo bufferViewCreateInfo;

    VkBufferView bufferView;

public:

    BufferView() = delete;
    BufferView(const VkDevice device, const VkBufferViewCreateFlags flags, const VkBuffer buffer, const VkFormat format, const VkDeviceSize offset, const VkDeviceSize range, const VkBufferView bufferView);
    BufferView(const BufferView& other) = delete;
    BufferView(BufferView&& other) = delete;
    virtual ~BufferView();

    BufferView& operator =(const BufferView& other) = delete;

    BufferView& operator =(BufferView && other) = delete;

    //
    // IBufferView
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkBufferViewCreateInfo& getBufferViewCreateInfo() const override;

    virtual VkBufferViewCreateFlags getFlags() const override;

    virtual const VkBuffer getBuffer() const override;

    virtual VkFormat getFormat() const override;

    virtual VkDeviceSize getOffset() const override;

    virtual VkDeviceSize getRange() const override;

    virtual const VkBufferView getBufferView() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_BUFFERVIEW_HPP_ */
