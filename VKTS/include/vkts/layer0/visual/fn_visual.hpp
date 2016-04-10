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

#ifndef VKTS_FN_VISUAL_HPP_
#define VKTS_FN_VISUAL_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

/**
 * Not thread Safe.
 */
VKTS_APICALL VkBool32 VKTS_APIENTRY visualInit(const VkInstance instance, const VkPhysicalDevice physicalDevice);

//

/**
 * Not thread Safe.
 */
VKTS_APICALL int32_t VKTS_APIENTRY visualGetNumberDisplays();

/**
 * Not thread Safe.
 */
VKTS_APICALL INativeDisplayWP VKTS_APIENTRY visualCreateDefaultDisplay();

/**
 * Not thread Safe.
 */
VKTS_APICALL INativeDisplayWP VKTS_APIENTRY visualCreateDisplay(const int32_t displayIndex);

/**
 * Not thread Safe.
 */
VKTS_APICALL INativeDisplayWP VKTS_APIENTRY visualGetDisplay(const int32_t displayIndex);

//

/**
 * Not thread Safe.
 */
VKTS_APICALL INativeWindowWP VKTS_APIENTRY visualCreateWindow(const INativeDisplayWP& display, const char* title, const int32_t width, const int32_t height, const VkBool32 fullscreen, const VkBool32 resize, const VkBool32 invisibleCursor);

/**
 * Not thread Safe.
 */
VKTS_APICALL INativeWindowWP VKTS_APIENTRY visualGetWindow(const int32_t windowIndex);

//

/**
 * Not thread Safe.
 */
VKTS_APICALL void VKTS_APIENTRY visualTerminate();

}

#endif /* VKTS_FN_VISUAL_HPP_ */
