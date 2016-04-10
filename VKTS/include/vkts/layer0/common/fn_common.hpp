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

#ifndef VKTS_FN_COMMON_HPP_
#define VKTS_FN_COMMON_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL VkBool32 VKTS_APIENTRY commonGetMemoryTypeIndex(const uint32_t memoryTypeCount, const VkMemoryType* memoryType, const uint32_t memoryTypeBits, const VkMemoryPropertyFlags propertyFlags, uint32_t& memoryTypeIndex);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL VkDeviceSize VKTS_APIENTRY commonGetDeviceSize(const size_t currentSize, const size_t stride);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL int32_t VKTS_APIENTRY commonGetOffsetInBytes(const VkTsVertexBufferType element, const VkTsVertexBufferType allElements);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL uint32_t VKTS_APIENTRY commonGetStrideInBytes(const VkTsVertexBufferType allElements);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL VkBool32 VKTS_APIENTRY commonIsUNORM(const VkFormat format);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL VkBool32 VKTS_APIENTRY commonIsSFLOAT(const VkFormat format);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL int32_t VKTS_APIENTRY commonGetBytesPerChannel(const VkFormat format);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL int32_t VKTS_APIENTRY commonGetNumberChannels(const VkFormat format);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL size_t VKTS_APIENTRY commonGetStructureTypeSize(const void* ptr);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL IBinaryBufferSP VKTS_APIENTRY commonSerializeStructureType(const void* ptr);

}

#endif /* VKTS_FN_COMMON_HPP_ */
