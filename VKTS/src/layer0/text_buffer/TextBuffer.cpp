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

#include "TextBuffer.hpp"

namespace vkts
{

TextBuffer::TextBuffer() :
    ITextBuffer(), text(""), pos(0)
{
}

TextBuffer::TextBuffer(const char* text) :
    ITextBuffer(), text(""), pos(0)
{
    if (text)
    {
        this->text = std::string(text);
    }
}

TextBuffer::TextBuffer(const std::string& text) :
    ITextBuffer(), text(text), pos(0)
{
}

TextBuffer::~TextBuffer()
{
}

const char* TextBuffer::getString() const
{
    return static_cast<const char*>(text.c_str());
}

size_t TextBuffer::getLength() const
{
    return text.length();
}

VkBool32 TextBuffer::seek(const int64_t offset, const VkTsSearch search)
{
    switch (search)
    {
        case VKTS_SEARCH_ABSOLUTE:
        {
            if (offset < 0 || offset > static_cast<int64_t>(text.length() + 1))
            {
                return VK_FALSE;
            }

            pos = offset;

            return VK_TRUE;
        }
        break;
        case VKTS_SEARCH_RELATVE:
        {
            if (offset < 0)
            {
                if (-offset > static_cast<int64_t>(pos))
                {
                    return VK_FALSE;
                }
            }
            else if (offset > 0)
            {
                if (offset >= static_cast<int64_t>(text.length() + 1 - pos))
                {
                    return VK_FALSE;
                }
            }

            pos += offset;

            return VK_TRUE;
        }
        break;
    }

    return VK_FALSE;
}

const char* TextBuffer::gets(char* str, const size_t num)
{
    if (!str || num == 0)
    {
        return nullptr;
    }

    if (pos == text.length() + 1)
    {
        return nullptr;
    }

    size_t strIndex = 0;

    while (strIndex < num)
    {
        str[strIndex] = text.c_str()[pos];

        if (pos == text.length() + 1)
        {
            return str;
        }
        else if (str[strIndex] == '\n')
        {
            str[strIndex] = '\0';

            pos++;

            return str;
        }

        strIndex++;
        pos++;
    }

    return str;
}

VkBool32 TextBuffer::puts(const char* str)
{
    if (!str)
    {
        return VK_FALSE;
    }

    if (pos == text.length() + 1)
    {
        text.append(str);

        pos = text.length() + 1;

        return VK_TRUE;
    }

    size_t strLen = strlen(str);

    size_t strIndex = 0;

    while (strIndex < strLen && pos < text.length())
    {
        text[pos] = str[strIndex];

        strIndex++;
        pos++;
    }

    while (strIndex < strLen)
    {
        text.push_back(str[strIndex]);

        strIndex++;
    }

    pos = text.length() + 1;

    return VK_TRUE;
}

//
// ICloneable
//

ITextBufferSP TextBuffer::clone() const
{
    return ITextBufferSP(new TextBuffer(text));
}

} /* namespace vkts */
