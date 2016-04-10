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

#ifndef VKTS_FN_VISUAL_DISPLAY_INTERNAL_HPP_
#define VKTS_FN_VISUAL_DISPLAY_INTERNAL_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

VKTS_APICALL VkBool32 VKTS_APIENTRY _visualInitDisplay(const VkInstance instance, const VkPhysicalDevice physicalDevice);

VKTS_APICALL VkBool32 VKTS_APIENTRY _visualDispatchMessagesDisplay();

VKTS_APICALL void VKTS_APIENTRY _visualTerminateDisplay();


VKTS_APICALL void VKTS_APIENTRY _visualSetDisplayExtent(const glm::ivec2& extent);


}
#endif /* VKTS_FN_VISUAL_DISPLAY_INTERNAL_HPP_ */
