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

#include "fn_visual_internal.hpp"

#include "NativeDisplay.hpp"

namespace vkts
{

NativeDisplay::NativeDisplay(const int32_t index, const VkBool32 defaultDisplay, const uint32_t width, const uint32_t height, VKTS_NATIVE_DISPLAY nativeDisplay) :
    INativeDisplay(), index(index), defaultDisplay(defaultDisplay), dimension(width, height), nativeDisplay(nativeDisplay)
{
}

NativeDisplay::~NativeDisplay()
{
    // No automatic destroy by purpose.
}

const glm::uvec2& NativeDisplay::getDimension() const
{
    return dimension;
}

void NativeDisplay::setDimension(const glm::uvec2& dimension)
{
    this->dimension = dimension;
}

//
// INativeDisplay
//

int32_t NativeDisplay::getIndex() const
{
    return index;
}

VkBool32 NativeDisplay::isDefault() const
{
    return defaultDisplay;
}

uint32_t NativeDisplay::getWidth() const
{
    return dimension.x;
}

uint32_t NativeDisplay::getHeight() const
{
    return dimension.y;
}

VKTS_NATIVE_DISPLAY NativeDisplay::getNativeDisplay() const
{
    return nativeDisplay;
}

//
// IDestroyable
//

void NativeDisplay::destroy()
{
    visualDestroyDisplay(shared_from_this());
}

} /* namespace vkts */
