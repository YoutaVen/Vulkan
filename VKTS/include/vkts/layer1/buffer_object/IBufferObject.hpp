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

#ifndef VKTS_IBUFFEROBJECT_HPP_
#define VKTS_IBUFFEROBJECT_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class IBufferObject: public IDestroyable
{

public:

    IBufferObject() :
        IDestroyable()
    {
    }

    virtual ~IBufferObject()
    {
    }

    virtual const IInitialResourcesSP& getInitialResources() const = 0;

    virtual const IBufferSP& getBuffer() const = 0;

    virtual const IBufferViewSP& getBufferView() const = 0;

    virtual const IDeviceMemorySP& getDeviceMemory() const = 0;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const void* data, const size_t size) const = 0;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const glm::mat4& mat) const = 0;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const glm::mat3& mat) const = 0;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const glm::mat2& mat) const = 0;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const glm::vec4& vec) const = 0;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const glm::vec3& vec) const = 0;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const glm::vec2& vec) const = 0;

    virtual VkBool32 upload(const size_t offset, const VkMemoryMapFlags flags, const float scalar) const = 0;

};

typedef std::shared_ptr<IBufferObject> IBufferObjectSP;

} /* namespace vkts */

#endif /* VKTS_IBUFFEROBJECT_HPP_ */
