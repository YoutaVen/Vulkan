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

#ifndef VKTS_BUFFEROBJECT_HPP_
#define VKTS_BUFFEROBJECT_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class BufferObject: public IBufferObject
{

private:

    const IInitialResourcesSP initialResources;

    IBufferSP buffer;

    IBufferViewSP bufferView;

    IDeviceMemorySP deviceMemory;

public:

    BufferObject() = delete;
    BufferObject(const IInitialResourcesSP& initialResources, const IBufferSP& buffer, const IBufferViewSP& bufferView, const IDeviceMemorySP& deviceMemory);
    BufferObject(const BufferObject& other) = delete;
    BufferObject(BufferObject&& other) = delete;
    virtual ~BufferObject();

    BufferObject& operator =(const BufferObject& other) = delete;
    BufferObject& operator =(BufferObject && other) = delete;

    //
    // IBufferObject
    //

    virtual const IInitialResourcesSP& getInitialResources() const override;

    virtual const IBufferSP& getBuffer() const override;

    virtual const IBufferViewSP& getBufferView() const override;

    virtual const IDeviceMemorySP& getDeviceMemory() const override;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const void* data, const size_t size) const override;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const glm::mat4& mat) const override;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const glm::mat3& mat) const override;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const glm::mat2& mat) const override;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const glm::vec4& vec) const override;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const glm::vec3& vec) const override;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const glm::vec2& vec) const override;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const float scalar) const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_BUFFEROBJECT_HPP_ */
