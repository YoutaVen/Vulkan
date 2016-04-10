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

#include "fn_visual_internal.hpp"

namespace vkts
{

VkBool32 VKTS_APIENTRY visualInit(const VkInstance instance, const VkPhysicalDevice physicalDevice)
{
    return _visualInit(instance, physicalDevice);
}

VkBool32 VKTS_APIENTRY visualDispatchMessages()
{
    return _visualDispatchMessages();
}

//

int32_t VKTS_APIENTRY visualGetNumberDisplays()
{
    return _visualGetNumberDisplays();
}

INativeDisplayWP VKTS_APIENTRY visualCreateDefaultDisplay()
{
    return _visualCreateDefaultDisplay();
}

INativeDisplayWP VKTS_APIENTRY visualCreateDisplay(const int32_t displayIndex)
{
    return _visualCreateDisplay(displayIndex);
}

const NativeDisplaySP VKTS_APIENTRY visualGetDisplayInternal(const int32_t displayIndex)
{
    return _visualGetDisplay(displayIndex);
}

INativeDisplayWP VKTS_APIENTRY visualGetDisplay(const int32_t displayIndex)
{
    return _visualGetDisplay(displayIndex);
}

void VKTS_APIENTRY visualDestroyDisplay(const NativeDisplaySP& display)
{
    _visualDestroyDisplay(display);
}

//

INativeWindowWP VKTS_APIENTRY visualCreateWindow(const INativeDisplayWP& display, const char* title, const int32_t width, const int32_t height, const VkBool32 fullscreen, const VkBool32 resize, const VkBool32 invisibleCursor)
{
    return _visualCreateWindow(display, title, width, height, fullscreen, resize, invisibleCursor);
}

const NativeWindowSP VKTS_APIENTRY visualGetWindowInternal(const int32_t windowIndex)
{
    return _visualGetWindow(windowIndex);
}

INativeWindowWP VKTS_APIENTRY visualGetWindow(const int32_t windowIndex)
{
    return _visualGetWindow(windowIndex);
}

void VKTS_APIENTRY visualDestroyWindow(const NativeWindowSP& window)
{
    _visualDestroyWindow(window);
}

//

void VKTS_APIENTRY visualTerminate()
{
    _visualTerminate();
}

}
