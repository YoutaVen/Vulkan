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

namespace vkts
{

VkBool32 VKTS_APIENTRY commonGetMemoryTypeIndex(const uint32_t memoryTypeCount, const VkMemoryType* memoryType, const uint32_t memoryTypeBits, const VkMemoryPropertyFlags propertyFlags, uint32_t& memoryTypeIndex)
{
    if (memoryTypeCount == 0 || memoryTypeCount > VK_MAX_MEMORY_TYPES || !memoryType || memoryTypeBits == 0)
    {
        return VK_FALSE;
    }

    uint32_t searchMemoryBits = memoryTypeBits;

    for (uint32_t currentMemoryTypeIndex = 0; currentMemoryTypeIndex < glm::max(32u, (uint32_t) VK_MAX_MEMORY_TYPES); currentMemoryTypeIndex++)
    {
        if (searchMemoryBits & 1)
        {
            if ((memoryType[currentMemoryTypeIndex].propertyFlags & propertyFlags) == propertyFlags)
            {
                memoryTypeIndex = currentMemoryTypeIndex;

                return VK_TRUE;
            }
        }

        // Lower bits first.
        searchMemoryBits >>= 1;
    }

    return VK_FALSE;
}

VkDeviceSize VKTS_APIENTRY commonGetDeviceSize(const size_t currentSize, const size_t stride)
{
    if (currentSize == 0 || stride == 0)
    {
        return 0;
    }

    return VkDeviceSize((currentSize / stride) * stride + ((currentSize % stride) > 0 ? stride : 0));
}

int32_t VKTS_APIENTRY commonGetOffsetInBytes(const VkTsVertexBufferType element, const VkTsVertexBufferType allElements)
{
    int32_t result = 0;

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_VERTEX)
    {
        if (VKTS_VERTEX_BUFFER_TYPE_VERTEX == element)
        {
            return result;
        }

        result += 4 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_NORMAL)
    {
        if (VKTS_VERTEX_BUFFER_TYPE_NORMAL == element)
        {
            return result;
        }

        result += 3 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_BITANGENT)
    {
        if (VKTS_VERTEX_BUFFER_TYPE_BITANGENT == element)
        {
            return result;
        }

        result += 3 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_TANGENT)
    {
        if (VKTS_VERTEX_BUFFER_TYPE_TANGENT == element)
        {
            return result;
        }

        result += 3 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_TEXCOORD)
    {
        if (VKTS_VERTEX_BUFFER_TYPE_TEXCOORD == element)
        {
            return result;
        }

        result += 2 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_BONE_INDICES0)
    {
        if (VKTS_VERTEX_BUFFER_TYPE_BONE_INDICES0 == element)
        {
            return result;
        }

        result += 4 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_BONE_INDICES1)
    {
        if (VKTS_VERTEX_BUFFER_TYPE_BONE_INDICES1 == element)
        {
            return result;
        }

        result += 4 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_BONE_WEIGHTS0)
    {
        if (VKTS_VERTEX_BUFFER_TYPE_BONE_WEIGHTS0 == element)
        {
            return result;
        }

        result += 4 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_BONE_WEIGHTS1)
    {
        if (VKTS_VERTEX_BUFFER_TYPE_BONE_WEIGHTS1 == element)
        {
            return result;
        }

        result += 4 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_BONE_NUMBERS)
    {
        if (VKTS_VERTEX_BUFFER_TYPE_BONE_NUMBERS == element)
        {
            return result;
        }

        result += 1 * sizeof(float);
    }

    return -1;
}

uint32_t VKTS_APIENTRY commonGetStrideInBytes(const VkTsVertexBufferType allElements)
{
    uint32_t result = 0;

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_VERTEX)
    {
        result += 4 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_NORMAL)
    {
        result += 3 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_BITANGENT)
    {
        result += 3 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_TANGENT)
    {
        result += 3 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_TEXCOORD)
    {
        result += 2 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_BONE_INDICES0)
    {
        result += 4 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_BONE_INDICES1)
    {
        result += 4 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_BONE_WEIGHTS0)
    {
        result += 4 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_BONE_WEIGHTS1)
    {
        result += 4 * sizeof(float);
    }

    if (allElements & VKTS_VERTEX_BUFFER_TYPE_BONE_NUMBERS)
    {
        result += 1 * sizeof(float);
    }

    return result;
}

VkBool32 VKTS_APIENTRY commonIsUNORM(const VkFormat format)
{
    switch (format)
    {
        case VK_FORMAT_R8_UNORM:
        case VK_FORMAT_R8G8_UNORM:
        case VK_FORMAT_R8G8B8_UNORM:
        case VK_FORMAT_B8G8R8_UNORM:
        case VK_FORMAT_R8G8B8A8_UNORM:
        case VK_FORMAT_B8G8R8A8_UNORM:
            return VK_TRUE;
        default:
            return VK_FALSE;
    }

    return VK_FALSE;
}

VkBool32 VKTS_APIENTRY commonIsSFLOAT(const VkFormat format)
{
    switch (format)
    {
        case VK_FORMAT_R32_SFLOAT:
        case VK_FORMAT_R32G32_SFLOAT:
        case VK_FORMAT_R32G32B32_SFLOAT:
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            return VK_TRUE;
        default:
            return VK_FALSE;
    }

    return VK_FALSE;
}

int32_t VKTS_APIENTRY commonGetBytesPerChannel(const VkFormat format)
{
    switch (format)
    {
        case VK_FORMAT_R8_UNORM:
        case VK_FORMAT_R8G8_UNORM:
        case VK_FORMAT_R8G8B8_UNORM:
        case VK_FORMAT_B8G8R8_UNORM:
        case VK_FORMAT_R8G8B8A8_UNORM:
        case VK_FORMAT_B8G8R8A8_UNORM:
            return 1;

        case VK_FORMAT_R32_SFLOAT:
        case VK_FORMAT_R32G32_SFLOAT:
        case VK_FORMAT_R32G32B32_SFLOAT:
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            return 4;
        default:
            return 0;
    }

    return 0;
}

int32_t VKTS_APIENTRY commonGetNumberChannels(const VkFormat format)
{
    switch (format)
    {
        case VK_FORMAT_R8_UNORM:
            return 1;
        case VK_FORMAT_R8G8_UNORM:
            return 2;
        case VK_FORMAT_R8G8B8_UNORM:
            return 3;
        case VK_FORMAT_B8G8R8_UNORM:
            return 3;
        case VK_FORMAT_R8G8B8A8_UNORM:
            return 4;
        case VK_FORMAT_B8G8R8A8_UNORM:
            return 4;

        case VK_FORMAT_R32_SFLOAT:
            return 1;
        case VK_FORMAT_R32G32_SFLOAT:
            return 2;
        case VK_FORMAT_R32G32B32_SFLOAT:
            return 3;
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            return 4;
        default:
            return 0;
    }

    return 0;
}

size_t VKTS_APIENTRY commonGetStructureTypeSize(const void* ptr)
{
    if (!ptr)
    {
        return 0;
    }

    const VkTsStructureTypeHeader* structureTypeHeader = (const VkTsStructureTypeHeader*) ptr;

    switch (structureTypeHeader->sType)
    {
        case VK_STRUCTURE_TYPE_APPLICATION_INFO:
            return sizeof(VkApplicationInfo);
        case VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO:
            return sizeof(VkInstanceCreateInfo);
        case VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO:
            return sizeof(VkDeviceQueueCreateInfo);
        case VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO:
            return sizeof(VkDeviceCreateInfo);
        case VK_STRUCTURE_TYPE_SUBMIT_INFO:
            return sizeof(VkSubmitInfo);
        case VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO:
            return sizeof(VkMemoryAllocateInfo);
        case VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE:
            return sizeof(VkMappedMemoryRange);
        case VK_STRUCTURE_TYPE_BIND_SPARSE_INFO:
            return sizeof(VkBindSparseInfo);
        case VK_STRUCTURE_TYPE_FENCE_CREATE_INFO:
            return sizeof(VkFenceCreateInfo);
        case VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO:
            return sizeof(VkSemaphoreCreateInfo);
        case VK_STRUCTURE_TYPE_EVENT_CREATE_INFO:
            return sizeof(VkEventCreateInfo);
        case VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO:
            return sizeof(VkQueryPoolCreateInfo);
        case VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO:
            return sizeof(VkBufferCreateInfo);
        case VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO:
            return sizeof(VkBufferViewCreateInfo);
        case VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO:
            return sizeof(VkImageCreateInfo);
        case VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO:
            return sizeof(VkImageViewCreateInfo);
        case VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO:
            return sizeof(VkShaderModuleCreateInfo);
        case VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO:
            return sizeof(VkPipelineCacheCreateInfo);
        case VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO:
            return sizeof(VkPipelineShaderStageCreateInfo);
        case VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO:
            return sizeof(VkPipelineVertexInputStateCreateInfo);
        case VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO:
            return sizeof(VkPipelineInputAssemblyStateCreateInfo);
        case VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO:
            return sizeof(VkPipelineTessellationStateCreateInfo);
        case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO:
            return sizeof(VkPipelineViewportStateCreateInfo);
        case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO:
            return sizeof(VkPipelineRasterizationStateCreateInfo);
        case VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO:
            return sizeof(VkPipelineMultisampleStateCreateInfo);
        case VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO:
            return sizeof(VkPipelineDepthStencilStateCreateInfo);
        case VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO:
            return sizeof(VkPipelineColorBlendStateCreateInfo);
        case VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO:
            return sizeof(VkPipelineDynamicStateCreateInfo);
        case VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO:
            return sizeof(VkGraphicsPipelineCreateInfo);
        case VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO:
            return sizeof(VkComputePipelineCreateInfo);
        case VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO:
            return sizeof(VkPipelineLayoutCreateInfo);
        case VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO:
            return sizeof(VkSamplerCreateInfo);
        case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO:
            return sizeof(VkDescriptorSetLayoutCreateInfo);
        case VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO:
            return sizeof(VkDescriptorPoolCreateInfo);
        case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO:
            return sizeof(VkDescriptorSetAllocateInfo);
        case VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET:
            return sizeof(VkWriteDescriptorSet);
        case VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET:
            return sizeof(VkCopyDescriptorSet);
        case VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO:
            return sizeof(VkFramebufferCreateInfo);
        case VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO:
            return sizeof(VkRenderPassCreateInfo);
        case VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO:
            return sizeof(VkCommandPoolCreateInfo);
        case VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO:
            return sizeof(VkCommandBufferAllocateInfo);
        case VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO:
            return sizeof(VkCommandBufferBeginInfo);
        case VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO:
            return sizeof(VkRenderPassBeginInfo);
        case VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER:
            return sizeof(VkBufferMemoryBarrier);
        case VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER:
            return sizeof(VkImageMemoryBarrier);
        case VK_STRUCTURE_TYPE_MEMORY_BARRIER:
            return sizeof(VkMemoryBarrier);

        default:
            logPrint(VKTS_LOG_WARNING, "StructureType: Structure type not found 0x%x", structureTypeHeader->sType);

            return 0;
    }

    logPrint(VKTS_LOG_WARNING, "StructureType: Structure type not found 0x%x", structureTypeHeader->sType);

    return 0;
}

IBinaryBufferSP VKTS_APIENTRY commonSerializeStructureType(const void* ptr)
{
    if (!ptr)
    {
        return IBinaryBufferSP();
    }

    size_t totalSize = 0;

    const VkTsStructureTypeHeader* sourceStructureTypeHeader = (const VkTsStructureTypeHeader*) ptr;

    while (sourceStructureTypeHeader)
    {
        totalSize += commonGetStructureTypeSize(sourceStructureTypeHeader);

        sourceStructureTypeHeader = (const VkTsStructureTypeHeader*) sourceStructureTypeHeader->pNext;
    }

    //

    std::unique_ptr<uint8_t[]> data = std::unique_ptr<uint8_t[]>(new uint8_t[totalSize]);

    if (!data.get())
    {
        return IBinaryBufferSP();
    }

    //

    sourceStructureTypeHeader = (const VkTsStructureTypeHeader*) ptr;

    VkTsStructureTypeHeader* targetStructureTypeHeader = nullptr;

    size_t currentSize = 0;
    size_t currentOffset = 0;

    while (sourceStructureTypeHeader)
    {
        targetStructureTypeHeader = (VkTsStructureTypeHeader*) &data[currentOffset];

        currentSize = commonGetStructureTypeSize(sourceStructureTypeHeader);

        memcpy(targetStructureTypeHeader, sourceStructureTypeHeader, currentSize);

        currentOffset += currentSize;

        if (sourceStructureTypeHeader->pNext)
        {
            targetStructureTypeHeader->pNext = &data[currentOffset];
        }

        sourceStructureTypeHeader = (const VkTsStructureTypeHeader*) sourceStructureTypeHeader->pNext;
    }

    return binaryBufferCreate(&data[0], totalSize);
}

}
