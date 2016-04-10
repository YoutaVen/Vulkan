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

#ifndef VKTS_PHYSICALDEVICE_HPP_
#define VKTS_PHYSICALDEVICE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class PhysicalDevice: public IPhysicalDevice
{

private:

    VkPhysicalDevice physicalDevice;

    const uint32_t index;

    const VkInstance instance;

    std::vector<VkQueueFamilyProperties> allQueueFamilyProperties;

public:

    PhysicalDevice() = delete;
    PhysicalDevice(const VkPhysicalDevice physicalDevice, const uint32_t index, const VkInstance instance);
    PhysicalDevice(const PhysicalDevice& other) = delete;
    PhysicalDevice(PhysicalDevice&& other) = delete;
    virtual ~PhysicalDevice();

    PhysicalDevice& operator =(const PhysicalDevice& other) = delete;

    PhysicalDevice& operator =(PhysicalDevice && other) = delete;

    //
    // IPhysicalDevice
    //

    virtual const VkPhysicalDevice getPhysicalDevice() const override;

    virtual uint32_t getIndex() const override;

    virtual const VkInstance getInstance() const override;

    virtual void getPhysicalDeviceFeatures(VkPhysicalDeviceFeatures& physicalDeviceFeature) const override;

    virtual void getPhysicalDeviceProperties(VkPhysicalDeviceProperties& physicalDeviceProperties) const override;

    virtual void getPhysicalDeviceMemoryProperties(VkPhysicalDeviceMemoryProperties& physicalDeviceMemoryProperties) const override;

    virtual void getGetPhysicalDeviceFormatProperties(VkFormatProperties& formatProperties, const VkFormat format) const override;

    virtual void getGetPhysicalDeviceImageFormatProperties(VkImageFormatProperties& imageFormatProperties, const VkFormat format, const VkImageType type, const VkImageTiling tiling, const VkImageUsageFlags usage, const VkImageCreateFlags flags) const override;

    virtual VkBool32 isImageTilingAvailable(const VkImageTiling imageTiling, const VkFormat format, const VkImageType type, const VkImageCreateFlags flags, const VkExtent3D& extent, const uint32_t mipLevels, const uint32_t arrayLayers, const VkSampleCountFlags sampleCounts, const VkDeviceSize resourceSize) override;

    virtual VkBool32 getGetImageTilingAndMemoryProperty(VkImageTiling& imageTiling, VkMemoryPropertyFlags& memoryPropertyFlags, const VkFormat format, const VkImageType type, const VkImageCreateFlags flags, const VkExtent3D& extent, const uint32_t mipLevels, const uint32_t arraySize, const VkSampleCountFlags sampleCounts, const VkDeviceSize resourceSize) override;

    virtual const std::vector<VkQueueFamilyProperties>& getAllQueueFamilyProperties(const VkBool32 refresh = VK_FALSE) override;

};

} /* namespace vkts */

#endif /* VKTS_PHYSICALDEVICE_HPP_ */
