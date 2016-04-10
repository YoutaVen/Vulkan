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

#ifndef VKTS_IDESCRIPTORSETS_HPP_
#define VKTS_IDESCRIPTORSETS_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class IDescriptorSets: public IDestroyable
{

public:

    IDescriptorSets() :
        IDestroyable()
    {
    }

    virtual ~IDescriptorSets()
    {
    }

    virtual const VkDevice getDevice() const = 0;

    virtual const VkDescriptorPool getDescriptorPool() const = 0;

    virtual uint32_t getDescriptorSetCount() const = 0;

    virtual const VkDescriptorSetLayout* getSetLayouts() const = 0;

    virtual const VkDescriptorSet* getDescriptorSets() const = 0;

    virtual void updateDescriptorSets(const uint32_t writeCount, const VkWriteDescriptorSet* descriptorWrites, const uint32_t copyCount, const VkCopyDescriptorSet* descriptorCopies) const = 0;

    //
    // IDestroyable
    //

    virtual void destroy() = 0;

};

typedef std::shared_ptr<IDescriptorSets> IDescriptorSetsSP;

} /* namespace vkts */

#endif /* VKTS_IDESCRIPTORSETS_HPP_ */
