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

#ifndef VKTS_NATIVEDISPLAY_HPP_
#define VKTS_NATIVEDISPLAY_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class NativeDisplay: public INativeDisplay, public std::enable_shared_from_this<NativeDisplay>
{

private:

    const int32_t index;

    const VkBool32 defaultDisplay;

    glm::uvec2 dimension;

    VKTS_NATIVE_DISPLAY nativeDisplay;

public:

    NativeDisplay() = delete;
    NativeDisplay(const NativeDisplay& other) = delete;
    NativeDisplay(NativeDisplay&& other) = delete;
    NativeDisplay(const int32_t index, const VkBool32 defaultDisplay, const uint32_t width, const uint32_t height, VKTS_NATIVE_DISPLAY nativeDisplay);
    virtual ~NativeDisplay();

    NativeDisplay& operator =(const NativeDisplay& other) = delete;
    NativeDisplay& operator =(NativeDisplay && other) = delete;

    const glm::uvec2& getDimension() const;

    void setDimension(const glm::uvec2& dimension);

    //
    // INativeDisplay
    //

    virtual int32_t getIndex() const override;

    virtual VkBool32 isDefault() const override;

    virtual uint32_t getWidth() const override;

    virtual uint32_t getHeight() const override;

    virtual VKTS_NATIVE_DISPLAY getNativeDisplay() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

typedef std::shared_ptr<NativeDisplay> NativeDisplaySP;

} /* namespace vkts */

#endif /* VKTS_NATIVEDISPLAY_HPP_ */
