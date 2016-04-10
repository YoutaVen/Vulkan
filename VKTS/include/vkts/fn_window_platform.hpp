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

#ifndef VKTS_WINDOW_PLATFORM_HPP_
#define VKTS_WINDOW_PLATFORM_HPP_

#include <vkts/vkts.hpp>

#if defined(VKTS_NO_VISUAL)

// No windowing.

#define VKTS_NATIVE_DISPLAY             void*
#define VKTS_NATIVE_WINDOW              void*

#elif defined(VKTS_DISPLAY_VISUAL)

#define VKTS_NATIVE_DISPLAY             VkDisplayKHR
#define VKTS_NATIVE_WINDOW              VkDisplaySurfaceCreateInfoKHR*

#elif defined(_WIN32)

// Windows windowing.

#define VKTS_NATIVE_DISPLAY             HINSTANCE
#define VKTS_NATIVE_WINDOW              HWND

#elif defined(__ANDROID__)

// Android windowing.

#define VKTS_NATIVE_DISPLAY             void*
#define VKTS_NATIVE_WINDOW              ANativeWindow*

#elif defined(_X11_XLIB_H_) && defined(__XCB_H__)

// X11 and XCB windowing.

#define VKTS_NATIVE_DISPLAY             Display*
#define VKTS_NATIVE_WINDOW              Window

#else

// Unsupported window platform!

#error "Unsupported window platform detected!"

#endif

#endif /* VKTS_WINDOW_PLATFORM_HPP_ */
