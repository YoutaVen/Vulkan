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

#include "Channel.hpp"

namespace vkts
{

Channel::Channel() :
    IChannel(), name(""), targetTransform(VKTS_TARGET_TRANSFORM_TRANSLATE), targetTransformElement(VKTS_TARGET_TRANSFORM_ELEMENT_X), allKeys(), allValues(), allHandles(), allInterpolators()
{
}

Channel::Channel(const Channel& other) :
    IChannel(), name(other.name), targetTransform(other.targetTransform), targetTransformElement(other.targetTransformElement), allKeys(other.allKeys), allValues(other.allValues), allHandles(other.allHandles), allInterpolators(other.allInterpolators)
{
}

Channel::~Channel()
{
    destroy();
}

//
// IChannel
//

const std::string& Channel::getName() const
{
    return name;
}

void Channel::setName(const std::string& name)
{
    this->name = name;
}

VkTsTargetTransform Channel::getTargetTransform() const
{
    return targetTransform;
}

void Channel::setTargetTransform(VkTsTargetTransform targetTransform)
{
    this->targetTransform = targetTransform;
}

VkTsTargetTransformElement Channel::getTargetTransformElement() const
{
    return targetTransformElement;
}

void Channel::setTargetTransformElement(VkTsTargetTransformElement targetTransformElement)
{
    this->targetTransformElement = targetTransformElement;
}

VkBool32 Channel::addEntry(const float key, const float value, const glm::vec4& handles, const VkTsInterpolator interpolator)
{
    size_t index = 0;

    while (index < allKeys.size())
    {
        if (key == allKeys[index])
        {
            return VK_FALSE;
        }
        else if (key < allKeys[index])
        {
            index--;

            break;
        }

        index++;
    }

    if (index == allKeys.size())
    {
        allKeys.push_back(key);
        allValues.push_back(value);
        allHandles.push_back(handles);
        allInterpolators.push_back(interpolator);
    }
    else
    {
        allKeys.insert(allKeys.begin() + index, key);
        allValues.insert(allValues.begin() + index, value);
        allHandles.insert(allHandles.begin() + index, handles);
        allInterpolators.insert(allInterpolators.begin() + index, interpolator);
    }

    return VK_TRUE;
}

VkBool32 Channel::removeEntry(const float key)
{
    size_t index = 0;

    while (index < allKeys.size())
    {
        if (key == allKeys[index])
        {
            allKeys.erase(allKeys.begin() + index);
            allValues.erase(allValues.begin() + index);
            allHandles.erase(allHandles.begin() + index);
            allInterpolators.erase(allInterpolators.begin() + index);

            return VK_TRUE;
        }
        else if (key > allKeys[index])
        {
            break;
        }

        index++;
    }

    return VK_FALSE;
}

size_t Channel::getNumberEntries() const
{
    return allKeys.size();
}

const std::vector<float>& Channel::getKeys() const
{
    return allKeys;
}

const std::vector<float>& Channel::getValues() const
{
    return allValues;
}

const std::vector<glm::vec4>& Channel::getHandles() const
{
    return allHandles;
}

const std::vector<VkTsInterpolator>& Channel::getInterpolators() const
{
    return allInterpolators;
}

//
// ICloneable
//

IChannelSP Channel::clone() const
{
    return IChannelSP(new Channel(*this));
}

//
// IDestroyable
//

void Channel::destroy()
{
    allKeys.clear();

    allValues.clear();

    allHandles.clear();

    allInterpolators.clear();
}

} /* namespace vkts */
