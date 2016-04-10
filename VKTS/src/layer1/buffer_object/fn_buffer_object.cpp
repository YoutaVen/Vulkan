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

#include "BufferObject.hpp"

namespace vkts
{

static VkBool32 bufferObjectPrepare(IBufferSP& buffer, IDeviceMemorySP& deviceMemory, const IInitialResourcesSP& initialResources, const VkBufferCreateInfo& bufferCreateInfo, const VkMemoryPropertyFlags memoryPropertyFlag)
{
    VkResult result;

    //

    buffer = bufferCreate(initialResources->getDevice()->getDevice(), bufferCreateInfo.flags, bufferCreateInfo.size, bufferCreateInfo.usage, bufferCreateInfo.sharingMode, bufferCreateInfo.queueFamilyIndexCount, bufferCreateInfo.pQueueFamilyIndices);

    if (!buffer.get())
    {
        logPrint(VKTS_LOG_ERROR, "BufferObject: Could not create buffer.");

        return VK_FALSE;
    }

    VkMemoryRequirements memoryRequirements;

    buffer->getBufferMemoryRequirements(memoryRequirements);

    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    initialResources->getPhysicalDevice()->getPhysicalDeviceMemoryProperties(physicalDeviceMemoryProperties);

    deviceMemory = deviceMemoryCreate(initialResources->getDevice()->getDevice(), memoryRequirements, physicalDeviceMemoryProperties.memoryTypeCount, physicalDeviceMemoryProperties.memoryTypes, memoryPropertyFlag);

    if (!deviceMemory.get())
    {
        logPrint(VKTS_LOG_ERROR, "BufferObject: Could not allocate memory.");

        return VK_FALSE;
    }

    result = buffer->bindBufferMemory(deviceMemory->getDeviceMemory(), 0);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "BufferObject: Could not bind buffer memory.");

        return VK_FALSE;
    }

    return VK_TRUE;
}

IBufferObjectSP VKTS_APIENTRY bufferObjectCreate(IBufferSP& stageBuffer, IDeviceMemorySP& stageDeviceMemory, const IInitialResourcesSP& initialResources, const ICommandBuffersSP& cmdBuffer, const IBinaryBufferSP& binaryBuffer, const VkBufferCreateInfo& bufferCreateInfo, const VkMemoryPropertyFlags memoryPropertyFlag)
{
    if (!initialResources.get() || !cmdBuffer.get() || !binaryBuffer.get())
    {
        return IBufferObjectSP();
    }

    //

    VkResult result;

    //

    IBufferSP buffer;

    IDeviceMemorySP deviceMemory;

    if (!bufferObjectPrepare(buffer, deviceMemory, initialResources, bufferCreateInfo, memoryPropertyFlag))
    {
        return IBufferObjectSP();
    }

    //

    if (memoryPropertyFlag & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    {
        result = deviceMemory->upload(0, deviceMemory->getAllocationSize(), 0, binaryBuffer->getData(), binaryBuffer->getSize());

        if (result != VK_SUCCESS)
        {
            logPrint(VKTS_LOG_ERROR, "BufferObject: Could not upload vertex data.");

            return IBufferObjectSP();
        }
    }
    else
    {
        if (!bufferObjectPrepare(stageBuffer, stageDeviceMemory, initialResources, bufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
        {
            logPrint(VKTS_LOG_ERROR, "BufferObject: Could not create vertex buffer.");

            return IBufferObjectSP();
        }

        result = stageDeviceMemory->upload(0, stageDeviceMemory->getAllocationSize(), 0, binaryBuffer->getData(), binaryBuffer->getSize());

        if (result != VK_SUCCESS)
        {
            logPrint(VKTS_LOG_ERROR, "BufferObject: Could not upload vertex data.");

            return IBufferObjectSP();
        }

        VkBufferCopy bufferCopy;

        bufferCopy.srcOffset = 0;
        bufferCopy.dstOffset = 0;
        bufferCopy.size = binaryBuffer->getSize();

        stageBuffer->copyBuffer(cmdBuffer->getCommandBuffer(), buffer->getBuffer(), buffer->getSize(), 1, &bufferCopy);
    }

    //

    IBufferViewSP noBufferView;

    auto newInstance = new BufferObject(initialResources, buffer, noBufferView, deviceMemory);

    if (!newInstance)
    {
        buffer->destroy();

        return IBufferObjectSP();
    }

    return IBufferObjectSP(newInstance);
}

IBufferObjectSP VKTS_APIENTRY bufferObjectCreate(const IInitialResourcesSP& initialResources, const VkBufferCreateInfo& bufferCreateInfo, const VkMemoryPropertyFlags memoryPropertyFlag)
{
    if (!initialResources.get())
    {
        return IBufferObjectSP();
    }

    //

    IBufferSP buffer;

    IDeviceMemorySP deviceMemory;

    if (!bufferObjectPrepare(buffer, deviceMemory, initialResources, bufferCreateInfo, memoryPropertyFlag))
    {
        return IBufferObjectSP();
    }

    //

    IBufferViewSP noBufferView;

    auto newInstance = new BufferObject(initialResources, buffer, noBufferView, deviceMemory);

    if (!newInstance)
    {
        buffer->destroy();

        return IBufferObjectSP();
    }

    return IBufferObjectSP(newInstance);
}

IBufferObjectSP VKTS_APIENTRY bufferObjectCreate(const IInitialResourcesSP& initialResources, const VkBufferCreateInfo& bufferCreateInfo, const VkMemoryPropertyFlags memoryPropertyFlag, const VkFormat format)
{
    if (!initialResources.get())
    {
        return IBufferObjectSP();
    }

    //

    IBufferSP buffer;

    IDeviceMemorySP deviceMemory;

    if (!bufferObjectPrepare(buffer, deviceMemory, initialResources, bufferCreateInfo, memoryPropertyFlag))
    {
        return IBufferObjectSP();
    }

    //

    auto bufferView = bufferViewCreate(initialResources->getDevice()->getDevice(), 0, buffer->getBuffer(), format, 0, bufferCreateInfo.size);

    if (!bufferView.get())
    {
        buffer->destroy();

        return IBufferObjectSP();
    }

    //

    auto newInstance = new BufferObject(initialResources, buffer, bufferView, deviceMemory);

    if (!newInstance)
    {
        bufferView->destroy();

        buffer->destroy();

        return IBufferObjectSP();
    }

    return IBufferObjectSP(newInstance);
}

}
