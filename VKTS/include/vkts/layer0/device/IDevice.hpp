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

#ifndef VKTS_IDEVICE_HPP_
#define VKTS_IDEVICE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class IDevice: public IDestroyable
{

public:

    IDevice() :
        IDestroyable()
    {
    }

    virtual ~IDevice()
    {
    }

    virtual const VkPhysicalDevice getPhysicalDevice() const = 0;

    virtual const VkDeviceCreateInfo& getDeviceCreateInfo() const = 0;

    virtual VkDeviceCreateFlags getFlags() const = 0;

    virtual uint32_t getQueueCreateInfoCount() const = 0;

    virtual const VkDeviceQueueCreateInfo* getQueueCreateInfos() const = 0;

    virtual uint32_t getEnabledLayerCount() const = 0;

    virtual const char* const* getEnabledLayerNames() const = 0;

    virtual uint32_t getEnabledExtensionCount() const = 0;

    virtual const char* const* getEnabledExtensionNames() const = 0;

    virtual const VkPhysicalDeviceFeatures* getEnabledFeatures() const = 0;

    virtual const VkDevice getDevice() const = 0;

};

typedef std::shared_ptr<IDevice> IDeviceSP;

} /* namespace vkts */

#endif /* VKTS_IDEVICE_HPP_ */
