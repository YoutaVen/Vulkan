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

VkBool32 VKTS_APIENTRY _visualInit(const VkInstance instance, const VkPhysicalDevice physicalDevice)
{
    // Nothing for now.

    return VK_TRUE;
}

VkBool32 VKTS_APIENTRY _visualDispatchMessages()
{
    // Nothing for now.

    return VK_TRUE;
}

//

int32_t VKTS_APIENTRY _visualGetNumberDisplays()
{
    // Nothing for now.

    return 0;
}

INativeDisplayWP VKTS_APIENTRY _visualCreateDefaultDisplay()
{
    // Nothing for now.

    return INativeDisplayWP();
}

INativeDisplayWP VKTS_APIENTRY _visualCreateDisplay(const int32_t displayIndex)
{
    // Nothing for now.

    return INativeDisplayWP();
}

NativeDisplaySP VKTS_APIENTRY _visualGetDisplay(const int32_t displayIndex)
{
    // Nothing for now.

    return NativeDisplaySP();
}

const SmartPointerVector<NativeDisplaySP>& VKTS_APIENTRY _visualGetActiveDisplays()
{
    static SmartPointerVector<NativeDisplaySP> displayList;

    return displayList;
}

void VKTS_APIENTRY _visualDestroyDisplay(const NativeDisplaySP& display)
{
    // Nothing for now.
}

//

INativeWindowWP VKTS_APIENTRY _visualCreateWindow(const INativeDisplayWP& display, const char* title, const int32_t width, const int32_t height, const VkBool32 fullscreen, const VkBool32 resize, const VkBool32 invisibleCursor)
{
    // Nothing for now.

    return INativeWindowWP();
}

NativeWindowSP VKTS_APIENTRY _visualGetWindow(const int32_t windowIndex)
{
    // Nothing for now.

    return NativeWindowSP();
}

const SmartPointerVector<NativeWindowSP>& VKTS_APIENTRY _visualGetActiveWindows()
{
    static SmartPointerVector<NativeWindowSP> windowList;

    return windowList;
}

void VKTS_APIENTRY _visualDestroyWindow(const NativeWindowSP& window)
{
    // Nothing for now.
}

//

void VKTS_APIENTRY _visualTerminate()
{
    // Nothing for now.
}

}
