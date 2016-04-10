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

#ifndef VKTS_IDEVICEMEMORY_HPP_
#define VKTS_IDEVICEMEMORY_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class IDeviceMemory: public IDestroyable
{

public:

    IDeviceMemory() :
        IDestroyable()
    {
    }

    virtual ~IDeviceMemory()
    {
    }

    virtual const VkDevice getDevice() const = 0;

    virtual const VkMemoryAllocateInfo& getMemoryAllocInfo() const = 0;

    virtual VkDeviceSize getAllocationSize() const = 0;

    virtual uint32_t getMemoryTypeIndex() const = 0;

    virtual VkMemoryType getMemoryType() const = 0;

    virtual uint32_t getMemoryTypeCount() const = 0;

    virtual const VkMemoryType* getMemoryTypes() const = 0;

    virtual VkMemoryPropertyFlags getMemoryPropertyFlags() const = 0;

    virtual const VkDeviceMemory getDeviceMemory() const = 0;

    virtual VkResult mapMemory(const VkDeviceSize offset, const VkDeviceSize size, const VkMemoryMapFlags flags) = 0;

    virtual void* getMemory() = 0;

    virtual void unmapMemory() = 0;

    virtual VkResult upload(const VkDeviceSize offset, const VkDeviceSize size, const VkMemoryMapFlags flags, const void* uploadData, const size_t uploadDataSize) = 0;

};

typedef std::shared_ptr<IDeviceMemory> IDeviceMemorySP;

} /* namespace vkts */

#endif /* VKTS_IDEVICEMEMORY_HPP_ */
