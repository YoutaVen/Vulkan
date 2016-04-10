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

#ifndef VKTS_FN_IMAGE_DATA_HPP_
#define VKTS_FN_IMAGE_DATA_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL IImageDataSP VKTS_APIENTRY imageDataLoad(const char* filename);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL VkBool32 VKTS_APIENTRY imageDataSave(const char* filename, const IImageDataSP& imageData);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL IImageDataSP VKTS_APIENTRY imageDataCreate(const std::string& name, const uint32_t width, const uint32_t height, const uint32_t depth, const VkImageType imageType, const VkFormat& format);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL IImageDataSP VKTS_APIENTRY imageDataCreate(const std::string& name, const uint32_t width, const uint32_t height, const uint32_t depth, const float red, const float green, const float blue, const float alpha, const VkImageType imageType, const VkFormat& format);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL IImageDataSP VKTS_APIENTRY imageDataConvert(const IImageDataSP& sourceImage, const VkFormat targetFormat, const std::string& name);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL IImageDataSP VKTS_APIENTRY imageDataCopy(const IImageDataSP& sourceImage, const std::string& name);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL IImageDataSP VKTS_APIENTRY imageDataMerge(const SmartPointerVector<IImageDataSP>& sourceImages, const std::string& name);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL SmartPointerVector<IImageDataSP> VKTS_APIENTRY imageDataMipmap(const IImageDataSP& sourceImage, VkBool32 addSourceAsCopy, const std::string& name);

}

#endif /* VKTS_FN_IMAGE_DATA_HPP_ */
