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

#include "Image.hpp"

namespace vkts
{

Image::Image(const VkDevice device, const VkImageCreateFlags flags, const VkImageType imageType, const VkFormat format, const VkExtent3D& extent, const uint32_t mipLevels, const uint32_t arrayLayers, const VkSampleCountFlagBits samples, const VkImageTiling tiling, const VkImageUsageFlags usage, const VkSharingMode sharingMode, const uint32_t queueFamilyIndexCount, const uint32_t* queueFamilyIndices, const VkImageLayout initialLayout, const VkAccessFlags accessMask, const VkImage image) :
    IImage(), device(device), imageCreateInfo{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, nullptr, flags, imageType, format, extent, mipLevels, arrayLayers, samples, tiling, usage, sharingMode, queueFamilyIndexCount, nullptr, initialLayout}, allQueueFamilyIndices(), imageLayout(initialLayout), accessMask(accessMask), image(image)
{
    if (queueFamilyIndices)
    {
        for (uint32_t i = 0; i < queueFamilyIndexCount; i++)
        {
            allQueueFamilyIndices.push_back(queueFamilyIndices[i]);
        }

        if (queueFamilyIndexCount > 0)
        {
            imageCreateInfo.pQueueFamilyIndices = &allQueueFamilyIndices[0];
        }
    }
}

Image::~Image()
{
    destroy();
}

//
// IImage
//

const VkDevice Image::getDevice() const
{
    return device;
}

const VkImageCreateInfo& Image::getImageCreateInfo() const
{
    return imageCreateInfo;
}

VkImageCreateFlags Image::getFlags() const
{
    return imageCreateInfo.flags;
}

VkImageType Image::getImageType() const
{
    return imageCreateInfo.imageType;
}

const VkFormat& Image::getFormat() const
{
    return imageCreateInfo.format;
}

const VkExtent3D& Image::getExtent() const
{
    return imageCreateInfo.extent;
}

int32_t Image::getWidth() const
{
    return imageCreateInfo.extent.width;
}

int32_t Image::getHeight() const
{
    return imageCreateInfo.extent.height;
}

int32_t Image::getDepth() const
{
    return imageCreateInfo.extent.depth;
}

uint32_t Image::getMipLevels() const
{
    return imageCreateInfo.mipLevels;
}

uint32_t Image::getArrayLayers() const
{
    return imageCreateInfo.arrayLayers;
}

VkSampleCountFlagBits Image::getSamples() const
{
    return imageCreateInfo.samples;
}

VkImageTiling Image::getTiling() const
{
    return imageCreateInfo.tiling;
}

VkImageUsageFlags Image::getUsage() const
{
    return imageCreateInfo.usage;
}

VkSharingMode Image::getSharingMode() const
{
    return imageCreateInfo.sharingMode;
}

uint32_t Image::getQueueFamilyCount() const
{
    return imageCreateInfo.queueFamilyIndexCount;
}

const uint32_t* Image::getQueueFamilyIndices() const
{
    return imageCreateInfo.pQueueFamilyIndices;
}

VkImageLayout Image::getInitialLayout() const
{
    return imageCreateInfo.initialLayout;
}

const VkImage Image::getImage() const
{
    return image;
}

VkAccessFlags Image::getAccessMask() const
{
	return accessMask;
}

VkImageLayout Image::getImageLayout() const
{
    return imageLayout;
}

void Image::getImageMemoryRequirements(VkMemoryRequirements& memoryRequirements) const
{
    vkGetImageMemoryRequirements(device, image, &memoryRequirements);
}

void Image::getImageSubresourceLayout(VkSubresourceLayout& subresourceLayout, const VkImageSubresource& imageSubresource) const
{
    vkGetImageSubresourceLayout(device, image, &imageSubresource, &subresourceLayout);
}

void Image::copyImage(const VkCommandBuffer cmdBuffer, const VkImage targetImage, const VkAccessFlags targetAccessMask, const VkImageLayout targetImageLayout, const VkImageCopy& imageCopy) const
{
	if (imageLayout == VK_IMAGE_LAYOUT_UNDEFINED || imageLayout == VK_IMAGE_LAYOUT_PREINITIALIZED)
	{
		logPrint(VKTS_LOG_WARNING, "Image: Source layout not allowed: %d", imageLayout);

		return;
	}

	if (targetImageLayout == VK_IMAGE_LAYOUT_UNDEFINED || targetImageLayout == VK_IMAGE_LAYOUT_PREINITIALIZED)
	{
		logPrint(VKTS_LOG_WARNING, "Image: Target layout not allowed: %d", targetImageLayout);

		return;
	}

	//

	VkImageMemoryBarrier imageMemoryBarrier;

    memset(&imageMemoryBarrier, 0, sizeof(VkImageMemoryBarrier));

    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

    imageMemoryBarrier.srcAccessMask = 0;       				// Defined later.
    imageMemoryBarrier.dstAccessMask = 0;       				// Defined later.
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;	// Defined later.
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_UNDEFINED;	// Defined later.
    imageMemoryBarrier.srcQueueFamilyIndex = 0;
    imageMemoryBarrier.dstQueueFamilyIndex = 0;
    imageMemoryBarrier.image = VK_NULL_HANDLE;      			// Defined later.
    imageMemoryBarrier.subresourceRange = {imageCopy.srcSubresource.aspectMask, imageCopy.srcSubresource.mipLevel, 1, imageCopy.srcSubresource.baseArrayLayer, 1};

    // Prepare source image for copy.

	imageMemoryBarrier.srcAccessMask = accessMask;
	imageMemoryBarrier.dstAccessMask = accessMask | VK_ACCESS_TRANSFER_READ_BIT;
    imageMemoryBarrier.oldLayout = imageLayout;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

    imageMemoryBarrier.image = image;

    vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

    // Prepare target image for copy.

    imageMemoryBarrier.srcAccessMask = targetAccessMask;
    imageMemoryBarrier.dstAccessMask = targetAccessMask | VK_ACCESS_TRANSFER_WRITE_BIT;
    imageMemoryBarrier.oldLayout = targetImageLayout;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

    imageMemoryBarrier.image = targetImage;

    vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

    // Copy image by command.

    vkCmdCopyImage(cmdBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, targetImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopy);

    // Revert back.

    imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask = targetAccessMask;

    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imageMemoryBarrier.newLayout = targetImageLayout;

    vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

	imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	imageMemoryBarrier.dstAccessMask = accessMask;

	imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    imageMemoryBarrier.newLayout = imageLayout;

    imageMemoryBarrier.image = image;

    vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
}

void Image::copyImageToBuffer(const VkCommandBuffer cmdBuffer, const VkBuffer dstBuffer, const uint32_t regionCount, const VkBufferImageCopy* regions, const VkImageSubresourceRange& subresourceRange) const
{
	if (imageLayout == VK_IMAGE_LAYOUT_UNDEFINED || imageLayout == VK_IMAGE_LAYOUT_PREINITIALIZED)
	{
		logPrint(VKTS_LOG_WARNING, "Image: Source layout not allowed: %d", imageLayout);

		return;
	}

	for (uint32_t i = 0; i < regionCount; i++)
	{
		//

		VkImageMemoryBarrier imageMemoryBarrier;

		memset(&imageMemoryBarrier, 0, sizeof(VkImageMemoryBarrier));

		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

		imageMemoryBarrier.srcAccessMask = 0;       				// Defined later.
		imageMemoryBarrier.dstAccessMask = 0;       				// Defined later.
		imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;	// Defined later.
		imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_UNDEFINED;	// Defined later.
		imageMemoryBarrier.srcQueueFamilyIndex = 0;
		imageMemoryBarrier.dstQueueFamilyIndex = 0;
		imageMemoryBarrier.image = VK_NULL_HANDLE;      			// Defined later.
		imageMemoryBarrier.subresourceRange = subresourceRange;

		// Prepare source image for copy.

		imageMemoryBarrier.srcAccessMask = accessMask;
		imageMemoryBarrier.dstAccessMask = accessMask | VK_ACCESS_TRANSFER_READ_BIT;
		imageMemoryBarrier.oldLayout = imageLayout;
		imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

		imageMemoryBarrier.image = image;

		vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

		// Copy image by command.

		vkCmdCopyImageToBuffer(cmdBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstBuffer, 1, &regions[i]);

		// Revert back.

		vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

		imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		imageMemoryBarrier.dstAccessMask = accessMask;

		imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		imageMemoryBarrier.newLayout = imageLayout;

		imageMemoryBarrier.image = image;

		vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	}
}

void Image::cmdPipelineBarrier(const VkCommandBuffer cmdBuffer, const VkAccessFlags dstAccessMask, const VkImageLayout newLayout, const VkImageSubresourceRange& subresourceRange)
{
	if (newLayout == VK_IMAGE_LAYOUT_UNDEFINED || newLayout == VK_IMAGE_LAYOUT_PREINITIALIZED)
	{
		logPrint(VKTS_LOG_WARNING, "Image: New layout not allowed: %d", newLayout);

		return;
	}

    VkImageMemoryBarrier imageMemoryBarrier;

    memset(&imageMemoryBarrier, 0, sizeof(VkImageMemoryBarrier));

    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

    imageMemoryBarrier.srcAccessMask = accessMask;
    imageMemoryBarrier.dstAccessMask = dstAccessMask;
    imageMemoryBarrier.oldLayout = imageLayout;
    imageMemoryBarrier.newLayout = newLayout;
    imageMemoryBarrier.srcQueueFamilyIndex = 0;
    imageMemoryBarrier.dstQueueFamilyIndex = 0;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange = subresourceRange;

    vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

    accessMask = dstAccessMask;
    imageLayout = newLayout;
}

//
// IDestroyable
//

void Image::destroy()
{
    if (image)
    {
        vkDestroyImage(device, image, nullptr);

        image = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
