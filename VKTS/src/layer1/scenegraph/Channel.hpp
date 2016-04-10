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

#ifndef VKTS_CHANNEL_HPP_
#define VKTS_CHANNEL_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Channel: public IChannel
{

private:

    std::string name;

    VkTsTargetTransform targetTransform;

    VkTsTargetTransformElement targetTransformElement;

    std::vector<float> allKeys;

    std::vector<float> allValues;

    std::vector<glm::vec4> allHandles;

    std::vector<VkTsInterpolator> allInterpolators;

public:

    Channel();
    Channel(const Channel& other);
    Channel(Channel&& other) = delete;
    virtual ~Channel();

    Channel& operator =(const Channel& other) = delete;

    Channel& operator =(Channel && other) = delete;

    //
    // IChannel
    //

    virtual const std::string& getName() const override;

    virtual void setName(const std::string& name) override;

    virtual VkTsTargetTransform getTargetTransform() const override;

    virtual void setTargetTransform(VkTsTargetTransform targetTransform) override;

    virtual VkTsTargetTransformElement getTargetTransformElement() const override;

    virtual void setTargetTransformElement(VkTsTargetTransformElement targetTransformElement) override;

    virtual VkBool32 addEntry(const float key, const float value, const glm::vec4& handles, const VkTsInterpolator interpolator) override;

    virtual VkBool32 removeEntry(const float key) override;

    virtual size_t getNumberEntries() const override;

    virtual const std::vector<float>& getKeys() const override;

    virtual const std::vector<float>& getValues() const override;

    virtual const std::vector<glm::vec4>& getHandles() const override;

    virtual const std::vector<VkTsInterpolator>& getInterpolators() const override;

    //
    // ICloneable
    //

    virtual IChannelSP clone() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_CHANNEL_HPP_ */
