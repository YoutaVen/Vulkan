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

#include "../../fn_internal.hpp"

#include "../scenegraph/Channel.hpp"

namespace vkts
{

static float interpolateLinear(const size_t currentIndex, const float key, const IChannelSP& channel)
{
    float beforeKey = channel->getKeys()[currentIndex];
    float beforeValue = channel->getValues()[currentIndex];

    float afterKey = channel->getKeys()[currentIndex + 1];

    float deltaKey = afterKey - beforeKey;
    if (deltaKey == 0.0f)
    {
        return beforeValue;
    }

    float afterValue = channel->getValues()[currentIndex + 1];

    return (afterValue - beforeValue) * (key - beforeKey) / deltaKey + beforeValue;
}

static float interpolateBezier(const size_t currentIndex, const float key, const IChannelSP& channel)
{
    float beforeKey = channel->getKeys()[currentIndex];
    float beforeValue = channel->getValues()[currentIndex];

    float afterKey = channel->getKeys()[currentIndex + 1];

    float deltaKey = afterKey - beforeKey;
    if (deltaKey == 0.0f)
    {
        return beforeValue;
    }

    float afterValue = channel->getValues()[currentIndex + 1];

    float beforeRightHandleKey = channel->getHandles()[currentIndex].z;
    float afterLeftHandleKey = channel->getHandles()[currentIndex + 1].x;

    float t;
    float t2;
    float t3;
    float ot;
    float ot2;
    float ot3;

    float currentKey = key;
    float newKey;

    bool doBinarySearch = true;

    int32_t counter = 0;

    do
    {
        t = (currentKey - beforeKey) / deltaKey;
        t2 = t * t;
        t3 = t2 * t;
        ot = 1.0f - t;
        ot2 = ot * ot;
        ot3 = ot2 * ot;

        newKey = ot3 * beforeKey + 3.0f * ot2 * t * beforeRightHandleKey + 3.0f * ot * t2 * afterLeftHandleKey + t3 * afterKey;

        if (newKey > key + VKTS_BEZIER_TOLERANCE || newKey < key - VKTS_BEZIER_TOLERANCE)
        {
        	if ((newKey < currentKey && newKey > key) || (newKey > currentKey && newKey < key))
        	{
        		currentKey = (key + newKey) * 0.5f;
        	}
        	else if (newKey > currentKey)
        	{
        		currentKey -= VKTS_BEZIER_TOLERANCE;
        	}
        	else
        	{
        		currentKey += VKTS_BEZIER_TOLERANCE;
        	}
        }
        else
        {
        	currentKey = newKey;

            doBinarySearch = false;
        }

        counter++;
    }
    while (doBinarySearch && counter < VKTS_BEZIER_LOOPS);

    float beforeRightHandleValue = channel->getHandles()[currentIndex].w;
    float afterLeftHandleValue = channel->getHandles()[currentIndex + 1].y;

    t = (currentKey - beforeKey) / deltaKey;
    t2 = t * t;
    t3 = t2 * t;
    ot = 1.0f - t;
    ot2 = ot * ot;
    ot3 = ot2 * ot;

    return ot3 * beforeValue + 3.0f * ot2 * t * beforeRightHandleValue + 3.0f * ot * t2 * afterLeftHandleValue + t3 * afterValue;
}

float VKTS_APIENTRY interpolate(const float key, const IChannelSP& channel)
{
    if (!channel.get())
    {
        return 0.0f;
    }

    if (channel->getNumberEntries() == 0)
    {
        return 0.0f;
    }

    if (channel->getNumberEntries() == 1)
    {
        return channel->getValues()[0];
    }

    if (key <= channel->getKeys()[0])
    {
        return channel->getValues()[0];
    }

    auto lastIndex = channel->getNumberEntries() - 1;

    if (key >= channel->getKeys()[lastIndex])
    {
        return channel->getValues()[lastIndex];
    }

    // Key is in between the available keys now.

    size_t currentIndex = 0;
    while (currentIndex < channel->getNumberEntries())
    {
        if (key < channel->getKeys()[currentIndex])
        {
        	currentIndex--;

            break;
        }

        currentIndex++;
    }

    if (channel->getInterpolators()[currentIndex] == VKTS_INTERPOLATOR_BEZIER)
    {
        if (channel->getInterpolators()[currentIndex + 1] == VKTS_INTERPOLATOR_BEZIER)
        {
            return interpolateBezier(currentIndex, key, channel);
        }

        return interpolateLinear(currentIndex, key, channel);
    }

    if (channel->getInterpolators()[currentIndex] == VKTS_INTERPOLATOR_LINEAR)
    {
        return interpolateLinear(currentIndex, key, channel);
    }

    // VKTS_INTERPOLATOR_CONSTANT
    return channel->getValues()[currentIndex];
}

IChannelSP VKTS_APIENTRY interpolateConvert(const IChannelSP& channel, const float sampleTime)
{
    if (!channel.get() || sampleTime <= 0.0f)
    {
        return IChannelSP();
    }

    IChannelSP converted = IChannelSP(new Channel());

    for (size_t i = 0; i < channel->getNumberEntries(); i++)
    {
        if (channel->getInterpolators()[i] != VKTS_INTERPOLATOR_BEZIER)
        {
            converted->addEntry(channel->getKeys()[i], channel->getValues()[i], channel->getHandles()[i], channel->getInterpolators()[i]);
        }
        else
        {
            if (i == channel->getNumberEntries() - 1)
            {
                converted->addEntry(channel->getKeys()[i], channel->getValues()[i], channel->getHandles()[i], VKTS_INTERPOLATOR_LINEAR);
            }
            else
            {
                converted->addEntry(channel->getKeys()[i], channel->getValues()[i], channel->getHandles()[i], VKTS_INTERPOLATOR_LINEAR);

                auto currentKey = channel->getKeys()[i];
                auto nextKey = channel->getKeys()[i + 1];

                currentKey += sampleTime;

                while (currentKey < nextKey)
                {
                    auto currentValue = interpolate(currentKey, channel);

                    converted->addEntry(currentKey, currentValue, glm::vec4(currentKey - 0.1f, currentValue, currentKey + 0.1f, currentValue), VKTS_INTERPOLATOR_LINEAR);

                    currentKey += sampleTime;
                }
            }
        }
    }

    return converted;
}

}
