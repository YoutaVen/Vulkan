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

#include <vkts/vkts.hpp>

#include "MemoryImage.hpp"

namespace vkts
{

static VkBool32 memoryImageUpload(const IDeviceMemorySP& deviceMemory, const IImageDataSP& imageData, const uint32_t mipLevel, const VkSubresourceLayout& subresourceLayout)
{
    if (!imageData.get())
    {
        return VK_FALSE;
    }

    VkResult result;

    if (deviceMemory->getMemoryPropertyFlags() & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    {
        result = deviceMemory->mapMemory(0, deviceMemory->getAllocationSize(), 0);

        if (result != VK_SUCCESS)
        {
            logPrint(VKTS_LOG_ERROR, "MemoryImage: Could not map memory.");

            return VK_FALSE;
        }

        imageData->copy(deviceMemory->getMemory(), mipLevel, subresourceLayout);

        deviceMemory->unmapMemory();
    }
    else
    {
        return VK_FALSE;
    }

    return VK_TRUE;
}

static VkBool32 memoryImagePrepare(IImageSP& image, IDeviceMemorySP& deviceMemory, const IInitialResourcesSP& initialResources, const ICommandBuffersSP& cmdBuffer, const VkImageCreateInfo& imageCreateInfo, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkImageLayout newLayout, const VkImageSubresourceRange& subresourceRange, const VkMemoryPropertyFlags memoryPropertyFlags)
{
    if (!cmdBuffer.get())
    {
        return VK_FALSE;
    }

    VkResult result;

    //

    image = imageCreate(initialResources->getDevice()->getDevice(), imageCreateInfo.flags, imageCreateInfo.imageType, imageCreateInfo.format, imageCreateInfo.extent, imageCreateInfo.mipLevels, imageCreateInfo.arrayLayers, imageCreateInfo.samples, imageCreateInfo.tiling, imageCreateInfo.usage, imageCreateInfo.sharingMode, imageCreateInfo.queueFamilyIndexCount, imageCreateInfo.pQueueFamilyIndices, imageCreateInfo.initialLayout, srcAccessMask);

    if (!image.get())
    {
        logPrint(VKTS_LOG_ERROR, "MemoryImage: Could not create image.");

        return VK_FALSE;
    }

    //

    VkMemoryRequirements memoryRequirements;

    image->getImageMemoryRequirements(memoryRequirements);

    //

    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    initialResources->getPhysicalDevice()->getPhysicalDeviceMemoryProperties(physicalDeviceMemoryProperties);

    deviceMemory = deviceMemoryCreate(initialResources->getDevice()->getDevice(), memoryRequirements, physicalDeviceMemoryProperties.memoryTypeCount, physicalDeviceMemoryProperties.memoryTypes, memoryPropertyFlags);

    if (!deviceMemory.get())
    {
        logPrint(VKTS_LOG_ERROR, "MemoryImage: Could not allocate memory.");

        return VK_FALSE;
    }

    //

    result = vkBindImageMemory(initialResources->getDevice()->getDevice(), image->getImage(), deviceMemory->getDeviceMemory(), 0);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "MemoryImage: Could not bind image memory.");

        return VK_FALSE;
    }

    //

    image->cmdPipelineBarrier(cmdBuffer->getCommandBuffer(), dstAccessMask, newLayout, subresourceRange);

    return VK_TRUE;
}

static VkBool32 memoryImagePrepare(IBufferSP& buffer, IDeviceMemorySP& deviceMemory, const IInitialResourcesSP& initialResources, const VkBufferCreateInfo& bufferCreateInfo, const VkMemoryPropertyFlags memoryPropertyFlags)
{

    VkResult result;

    //

    buffer = bufferCreate(initialResources->getDevice()->getDevice(), bufferCreateInfo.flags, bufferCreateInfo.size, bufferCreateInfo.usage, bufferCreateInfo.sharingMode, bufferCreateInfo.queueFamilyIndexCount, bufferCreateInfo.pQueueFamilyIndices);

    if (!buffer.get())
    {
        logPrint(VKTS_LOG_ERROR, "MemoryImage: Could not create buffer.");

        return VK_FALSE;
    }

    //

    VkMemoryRequirements memoryRequirements;

    buffer->getBufferMemoryRequirements(memoryRequirements);

    //

    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    initialResources->getPhysicalDevice()->getPhysicalDeviceMemoryProperties(physicalDeviceMemoryProperties);

    deviceMemory = deviceMemoryCreate(initialResources->getDevice()->getDevice(), memoryRequirements, physicalDeviceMemoryProperties.memoryTypeCount, physicalDeviceMemoryProperties.memoryTypes, memoryPropertyFlags);

    if (!deviceMemory.get())
    {
        logPrint(VKTS_LOG_ERROR, "MemoryImage: Could not allocate memory.");

        return VK_FALSE;
    }

    //

    result = vkBindBufferMemory(initialResources->getDevice()->getDevice(), buffer->getBuffer(), deviceMemory->getDeviceMemory(), 0);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "MemoryImage: Could not bind buffer memory.");

        return VK_FALSE;
    }

    return VK_TRUE;
}

IMemoryImageSP VKTS_APIENTRY memoryImageCreate(IImageSP& stageImage, IBufferSP& stageBuffer, IDeviceMemorySP& stageDeviceMemory, const IInitialResourcesSP& initialResources, const ICommandBuffersSP& cmdBuffer, const std::string& name, const IImageDataSP& imageData, const VkImageCreateInfo& imageCreateInfo, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkImageLayout newLayout, const VkImageSubresourceRange& subresourceRange, const VkMemoryPropertyFlags memoryPropertyFlags)
{
    if (!cmdBuffer || !imageData.get())
    {
        return IMemoryImageSP();
    }

    VkResult result;

    //

    IImageSP image;

    IDeviceMemorySP deviceMemory;

    //

    if (!memoryImagePrepare(image, deviceMemory, initialResources, cmdBuffer, imageCreateInfo, srcAccessMask, dstAccessMask, newLayout, subresourceRange, memoryPropertyFlags))
    {
        return IMemoryImageSP();
    }

    //

    if (memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    {
        for (uint32_t i = 0; i < imageData->getMipLevels(); i++)
        {
            VkImageSubresource imageSubresource;

            imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageSubresource.mipLevel = i;
            imageSubresource.arrayLayer = 0;

            VkSubresourceLayout subresourceLayout;

            image->getImageSubresourceLayout(subresourceLayout, imageSubresource);

            if (!memoryImageUpload(deviceMemory, imageData, i, subresourceLayout))
            {
                logPrint(VKTS_LOG_ERROR, "MemoryImage: Could not upload image data.");

                return IMemoryImageSP();
            }
        }
    }
    else
    {
        if (initialResources->getPhysicalDevice()->isImageTilingAvailable(VK_IMAGE_TILING_LINEAR, imageData->getFormat(), imageData->getImageType(), 0, imageData->getExtent3D(), imageData->getMipLevels(), 1, VK_SAMPLE_COUNT_1_BIT, imageData->getSize()))
        {
            VkImageCreateInfo stageImageCreateInfo;

            memcpy(&stageImageCreateInfo, &imageCreateInfo, sizeof(VkImageCreateInfo));

            stageImageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
            stageImageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            stageImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;

            if (!memoryImagePrepare(stageImage, stageDeviceMemory, initialResources, cmdBuffer, stageImageCreateInfo, VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_HOST_WRITE_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, subresourceRange, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
            {
                logPrint(VKTS_LOG_ERROR, "MemoryImage: Could not prepare staging image.");

                return IMemoryImageSP();
            }

            for (uint32_t i = 0; i < imageData->getMipLevels(); i++)
            {
                VkImageSubresource imageSubresource;
                imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                imageSubresource.mipLevel = i;
                imageSubresource.arrayLayer = 0;

                VkSubresourceLayout subresourceLayout;

                stageImage->getImageSubresourceLayout(subresourceLayout, imageSubresource);

                if (!memoryImageUpload(stageDeviceMemory, imageData, i, subresourceLayout))
                {
                    logPrint(VKTS_LOG_ERROR, "MemoryImage: Could not upload image data.");

                    return IMemoryImageSP();
                }

                VkImageCopy imageCopy;

                imageCopy.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, i, 0, 1};
                imageCopy.srcOffset = {0, 0, 0};
                imageCopy.dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, i, 0, 1};
                imageCopy.dstOffset = {0, 0, 0};
                imageCopy.extent = {glm::max(imageData->getWidth() >> i, 1u), glm::max(imageData->getHeight() >> i, 1u), glm::max(imageData->getDepth() >> i, 1u)};

                stageImage->copyImage(cmdBuffer->getCommandBuffer(), image->getImage(), image->getAccessMask(), image->getImageLayout(), imageCopy);
            }
        }
        else
        {
            VkBufferCreateInfo bufferCreateInfo;

            memset(&bufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

            bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferCreateInfo.size = imageData->getSize();
            bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
            bufferCreateInfo.flags = 0;
            bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            bufferCreateInfo.queueFamilyIndexCount = 0;
            bufferCreateInfo.pQueueFamilyIndices = nullptr;

            if (!memoryImagePrepare(stageBuffer, stageDeviceMemory, initialResources, bufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
            {
                logPrint(VKTS_LOG_ERROR, "MemoryImage: Could not prepare staging buffer.");

                return IMemoryImageSP();
            }

            // Copy image data into buffer.

            result = stageDeviceMemory->upload(0, stageDeviceMemory->getAllocationSize(), 0, imageData->getData(), imageData->getSize());

            if (result != VK_SUCCESS)
            {
                logPrint(VKTS_LOG_ERROR, "MemoryImage: Could not copy data to stage buffer.");

                return IMemoryImageSP();
            }

            // Upload from buffer into image.

            VkDeviceSize bufferOffset = 0;

            for (uint32_t i = 0; i < imageData->getMipLevels(); i++)
            {
                VkBufferImageCopy bufferImageCopy;

                bufferImageCopy.bufferOffset = bufferOffset;
                bufferImageCopy.bufferRowLength = glm::max(imageData->getWidth() >> i, 1u);
                bufferImageCopy.bufferImageHeight = glm::max(imageData->getHeight() >> i, 1u);
                bufferImageCopy.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, i, 0, 1};
                bufferImageCopy.imageOffset = {0, 0, 0};
                bufferImageCopy.imageExtent = {glm::max(imageData->getWidth() >> i, 1u), glm::max(imageData->getHeight() >> i, 1u), glm::max(imageData->getDepth() >> i, 1u)};

                stageBuffer->copyBufferToImage(cmdBuffer->getCommandBuffer(), image->getImage(), image->getImageLayout(), 1, &bufferImageCopy);

                //

                bufferOffset += (VkDeviceSize)(bufferImageCopy.imageExtent.width * bufferImageCopy.imageExtent.height * bufferImageCopy.imageExtent.depth) * (VkDeviceSize)(imageData->getBytesPerChannel() * imageData->getNumberChannels());
            }
        }
    }

    //

    auto newInstance = new MemoryImage(initialResources, name, imageData, image, deviceMemory);

    if (!newInstance)
    {
        newInstance->destroy();

        return IMemoryImageSP();
    }

    return IMemoryImageSP(newInstance);
}

IMemoryImageSP VKTS_APIENTRY memoryImageCreate(const IInitialResourcesSP& initialResources, const ICommandBuffersSP& cmdBuffer, const std::string& name, const VkImageCreateInfo& imageCreateInfo, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkImageLayout newLayout, const VkImageSubresourceRange& subresourceRange, const VkMemoryPropertyFlags memoryPropertyFlags)
{
    if (!cmdBuffer.get())
    {
        return IMemoryImageSP();
    }

    IImageSP image;

    IDeviceMemorySP deviceMemory;

    //

    if (!memoryImagePrepare(image, deviceMemory, initialResources, cmdBuffer, imageCreateInfo, srcAccessMask, dstAccessMask, newLayout, subresourceRange, memoryPropertyFlags))
    {
        return IMemoryImageSP();
    }

    //

    auto newInstance = new MemoryImage(initialResources, name, IImageDataSP(), image, deviceMemory);

    if (!newInstance)
    {
        newInstance->destroy();

        return IMemoryImageSP();
    }

    return IMemoryImageSP(newInstance);
}

}
