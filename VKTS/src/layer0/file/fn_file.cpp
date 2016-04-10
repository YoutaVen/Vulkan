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

#include "../binary_buffer/BinaryBuffer.hpp"

#include "../text_buffer/TextBuffer.hpp"

#include "fn_file_internal.hpp"

namespace vkts
{

static std::string g_baseDirectory = std::string("");

static VkBool32 fileSave(const char* filename, const void* data, const size_t size)
{
    if (!filename || !data || size == 0)
    {
        return VK_FALSE;
    }

    //

    if (!_filePrepareSaveBinary(filename))
    {
    	return VK_FALSE;
    }

    //

    FILE* file;

    std::string saveFilename = g_baseDirectory + std::string(filename);

    file = fopen(saveFilename.c_str(), "wb");

    if (!file)
    {
        return VK_FALSE;
    }

    size_t elementsWritten = fwrite(data, 1, size, file);

    fclose(file);

    return elementsWritten == size;
}

//

void VKTS_APIENTRY _fileSetBaseDirectory(const char* directory)
{
	g_baseDirectory = std::string(directory) + "/";
}

const char* VKTS_APIENTRY _fileGetBaseDirectory()
{
	return g_baseDirectory.c_str();
}

//

IBinaryBufferSP VKTS_APIENTRY fileLoadBinary(const char* filename)
{
    if (!filename)
    {
        return IBinaryBufferSP();
    }

    //

    if (!_filePrepareLoadBinary(filename))
    {
    	return IBinaryBufferSP();
    }

    //

    FILE* file;

    std::string loadFilename = g_baseDirectory + std::string(filename);

    //

    file = fopen(loadFilename.c_str(), "rb");

    if (!file)
    {
        return IBinaryBufferSP();
    }

    if (fseek(file, 0, SEEK_END))
    {
        fclose(file);

        return IBinaryBufferSP();
    }

    int64_t length = ftell(file);

    if (length < 0)
    {
        fclose(file);

        return IBinaryBufferSP();
    }

    size_t size = static_cast<size_t>(length);

    if (size == 0)
    {
        fclose(file);

        return IBinaryBufferSP();
    }

    auto data = new uint8_t[size];

    if (!data)
    {
        fclose(file);

        return IBinaryBufferSP();
    }

    memset(data, 0, size);

    rewind(file);

    auto elementsRead = fread(data, 1, size, file);

    if (elementsRead != size)
    {
        delete[] data;

        fclose(file);

        return IBinaryBufferSP();
    }

    fclose(file);

    auto buffer = IBinaryBufferSP(new BinaryBuffer(data, size));

    delete[] data;

    return buffer;
}

ITextBufferSP VKTS_APIENTRY fileLoadText(const char* filename)
{
    auto buffer = fileLoadBinary(filename);

    if (!buffer.get())
    {
        return ITextBufferSP();
    }

    if (!buffer->getData() || buffer->getSize() == 0)
    {
        return ITextBufferSP();
    }

    auto data = new char[buffer->getSize() + 1];

    if (!data)
    {
        return ITextBufferSP();
    }

    memset(data, 0, buffer->getSize() + 1);

    memcpy(data, buffer->getData(), buffer->getSize());

    auto text = ITextBufferSP(new TextBuffer(data));

    delete[] data;

    return text;
}

VkBool32 VKTS_APIENTRY fileSaveBinary(const char* filename, const IBinaryBufferSP& buffer)
{
    if (!buffer.get())
    {
        return VK_FALSE;
    }

    return fileSave(filename, buffer->getData(), buffer->getSize());
}

VkBool32 VKTS_APIENTRY fileSaveText(const char* filename, const ITextBufferSP& text)
{
    if (!text.get())
    {
        return VK_FALSE;
    }

    return fileSave(filename, text->getString(), text->getLength());
}

}
