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

#ifndef VKTS_BINARYBUFFER_HPP_
#define VKTS_BINARYBUFFER_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class BinaryBuffer: public IBinaryBuffer
{

private:

    uint8_t* data;

    size_t size;

    size_t pos;

    uint8_t* copyData(const uint8_t* source, const size_t size) const;

public:

    BinaryBuffer();
    BinaryBuffer(const size_t size);
    BinaryBuffer(const uint8_t* data, const size_t size);
    BinaryBuffer(const BinaryBuffer& other) = delete;
    BinaryBuffer(BinaryBuffer&& other) = delete;
    virtual ~BinaryBuffer();

    BinaryBuffer& operator =(const BinaryBuffer& other) = delete;
    BinaryBuffer& operator =(BinaryBuffer && other) = delete;

    //
    // IBinaryBuffer
    //

    virtual void reset() override;

    virtual const void* getData() const override;

    virtual size_t getSize() const override;

    virtual VkBool32 seek(const int64_t offset, const VkTsSearch search) override;

    virtual size_t read(void* ptr, const size_t sizeElement, const size_t countElement) override;

    virtual size_t write(const void* ptr, const size_t sizeElement, const size_t countElement) override;

    virtual VkBool32 copy(void* data) const override;

    //
    // ICloneable
    //

    virtual IBinaryBufferSP clone() const override;

};

} /* namespace vkts */

#endif /* VKTS_BINARYBUFFER_HPP_ */
