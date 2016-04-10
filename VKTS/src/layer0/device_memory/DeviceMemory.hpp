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

#ifndef VKTS_DEVICEMEMORY_HPP_
#define VKTS_DEVICEMEMORY_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class DeviceMemory: public IDeviceMemory
{

private:

    const VkDevice device;

    const VkMemoryAllocateInfo memoryAllocInfo;

    std::vector<VkMemoryType> allMemoryTypes;

    const VkMemoryPropertyFlags memoryPropertyFlags;

    VkDeviceMemory deviceMemory;

    void* data;
    VkBool32 mapped;

public:

    DeviceMemory() = delete;
    DeviceMemory(const VkDevice device, const VkMemoryAllocateInfo& memoryAllocInfo, const uint32_t memoryTypeCount, const VkMemoryType* memoryTypes, const VkMemoryPropertyFlags memoryPropertyFlags, const VkDeviceMemory deviceMemory);
    DeviceMemory(const DeviceMemory& other) = delete;
    DeviceMemory(DeviceMemory&& other) = delete;
    virtual ~DeviceMemory();

    DeviceMemory& operator =(const DeviceMemory& other) = delete;

    DeviceMemory& operator =(DeviceMemory && other) = delete;

    //
    // IDeviceMemory
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkMemoryAllocateInfo& getMemoryAllocInfo() const override;

    virtual VkDeviceSize getAllocationSize() const override;

    virtual uint32_t getMemoryTypeIndex() const override;

    virtual VkMemoryType getMemoryType() const override;

    virtual uint32_t getMemoryTypeCount() const override;

    virtual const VkMemoryType* getMemoryTypes() const override;

    virtual VkMemoryPropertyFlags getMemoryPropertyFlags() const override;

    virtual const VkDeviceMemory getDeviceMemory() const override;

    virtual VkResult mapMemory(const VkDeviceSize offset, const VkDeviceSize size, const VkMemoryMapFlags flags) override;

    virtual void* getMemory() override;

    virtual void unmapMemory() override;

    virtual VkResult upload(const VkDeviceSize offset, const VkDeviceSize size, const VkMemoryMapFlags flags, const void* uploadData, const size_t uploadDataSize) override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_DEVICEMEMORY_HPP_ */
