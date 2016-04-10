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

#ifndef VKTS_SURFACE_HPP_
#define VKTS_SURFACE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Surface: public ISurface
{

private:

    const VkInstance instance;

    const VKTS_NATIVE_DISPLAY nativeDisplay;

	const VKTS_NATIVE_WINDOW nativeWindow;

    VkSurfaceKHR surface;

public:

    Surface() = delete;
    Surface(const VkInstance instance,
    		const VKTS_NATIVE_DISPLAY nativeDisplay,
			const VKTS_NATIVE_WINDOW nativeWindow,
            const VkSurfaceKHR surface);
    Surface(const Surface& other) = delete;
    Surface(Surface&& other) = delete;
    virtual ~Surface();

    Surface& operator =(const Surface& other) = delete;
    Surface& operator =(Surface && other) = delete;

    //
    // ISurface
    //

    virtual const VkInstance getInstance() const override;

    virtual const VKTS_NATIVE_DISPLAY getNativeDisplay() const override;

    virtual const VKTS_NATIVE_WINDOW getNativeWindow() const override;

    virtual const VkSurfaceKHR getSurface() const override;

    //
    // IDestroyable
    //

    virtual void
    destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_SURFACE_HPP_ */
