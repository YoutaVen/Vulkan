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

#ifndef VKTS_DEVICE_HPP_
#define VKTS_DEVICE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Device: public IDevice
{

private:

    const VkPhysicalDevice physicalDevice;

    VkDeviceCreateInfo deviceCreateInfo;

    std::vector<VkDeviceQueueCreateInfo> allDeviceQueueCreateInfos;

    VkPhysicalDeviceFeatures physicalDeviceFeatures;

    VkDevice device;

public:

    Device() = delete;
    Device(const VkPhysicalDevice physicalDevice, const VkDeviceCreateFlags flags, const uint32_t queueCreateInfoCount, const VkDeviceQueueCreateInfo* queueCreateInfos, const uint32_t enabledLayerNameCount, const char* const* enabledLayerNames, const uint32_t enabledExtensionNameCount, const char* const* enabledExtensionNames, const VkPhysicalDeviceFeatures* enabledFeatures, const VkDevice device);
    Device(const Device& other) = delete;
    Device(Device&& other) = delete;
    virtual ~Device();

    Device& operator =(const Device& other) = delete;

    Device& operator =(Device && other) = delete;

    //
    // IDevice
    //

    virtual const VkPhysicalDevice getPhysicalDevice() const override;

    virtual const VkDeviceCreateInfo& getDeviceCreateInfo() const override;

    virtual VkDeviceCreateFlags getFlags() const override;

    virtual uint32_t getQueueCreateInfoCount() const override;

    virtual const VkDeviceQueueCreateInfo* getQueueCreateInfos() const override;

    virtual uint32_t getEnabledLayerCount() const override;

    virtual const char* const* getEnabledLayerNames() const override;

    virtual uint32_t getEnabledExtensionCount() const override;

    virtual const char* const* getEnabledExtensionNames() const override;

    virtual const VkPhysicalDeviceFeatures* getEnabledFeatures() const override;

    virtual const VkDevice getDevice() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_DEVICE_HPP_ */
