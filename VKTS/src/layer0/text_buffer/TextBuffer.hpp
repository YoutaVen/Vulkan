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

#ifndef VKTS_TEXTBUFFER_HPP_
#define VKTS_TEXTBUFFER_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class TextBuffer: public ITextBuffer
{

private:

    std::string text;

    size_t pos;

public:

    TextBuffer();
    TextBuffer(const char* text);
    TextBuffer(const std::string& text);
    TextBuffer(const TextBuffer& other);
    TextBuffer(TextBuffer&& other) = delete;
    virtual ~TextBuffer();

    TextBuffer& operator =(const TextBuffer& other) = delete;
    TextBuffer& operator =(TextBuffer && other) = delete;

    //
    // IText
    //

    virtual const char* getString() const override;

    virtual size_t getLength() const override;

    virtual VkBool32 seek(const int64_t offset, const VkTsSearch search) override;

    virtual const char* gets(char* str, const size_t num) override;

    virtual VkBool32 puts(const char* str) override;

    //
    // ICloneable
    //

    virtual ITextBufferSP clone() const override;

};

} /* namespace vkts */

#endif /* VKTS_TEXTBUFFER_HPP_ */
