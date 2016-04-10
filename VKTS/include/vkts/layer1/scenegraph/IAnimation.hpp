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

#ifndef VKTS_IANIMATION_HPP_
#define VKTS_IANIMATION_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class IAnimation: public ICloneable<IAnimation>, public IDestroyable
{

public:

    IAnimation() :
        ICloneable<IAnimation>(), IDestroyable()
    {
    }

    virtual ~IAnimation()
    {
    }

    virtual const std::string& getName() const = 0;

    virtual void setName(const std::string& name) = 0;

    virtual float getStart() const = 0;

    virtual void setStart(const float start) = 0;

    virtual float getStop() const = 0;

    virtual void setStop(const float stop) = 0;

    virtual void addChannel(const IChannelSP& channel) = 0;

    virtual VkBool32 removeChannel(const IChannelSP& channel) = 0;

    virtual size_t getNumberChannels() const = 0;

    virtual const SmartPointerVector<IChannelSP>& getChannels() const = 0;

};

typedef std::shared_ptr<IAnimation> IAnimationSP;

} /* namespace vkts */

#endif /* VKTS_IANIMATION_HPP_ */
