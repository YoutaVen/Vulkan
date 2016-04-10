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

#ifndef VKTS_PIPELINECACHE_HPP_
#define VKTS_PIPELINECACHE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class PipelineCache: public IPipelineCache
{

private:

    const VkDevice device;

    VkPipelineCacheCreateInfo pipelineCacheCreateInfo;

    IBinaryBufferSP initialData;

    VkPipelineCache pipelineCache;

public:

    PipelineCache() = delete;
    PipelineCache(const VkDevice device, const VkPipelineCacheCreateFlags flags, const size_t initialDataSize, const void* initialData, const VkPipelineCache pipelineCache);
    PipelineCache(const PipelineCache& other) = delete;
    PipelineCache(PipelineCache&& other) = delete;
    virtual ~PipelineCache();

    PipelineCache& operator =(const PipelineCache& other) = delete;
    PipelineCache& operator =(PipelineCache && other) = delete;

    //
    // IPipelineCache
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkPipelineCacheCreateInfo& getPipelineCacheCreateInfo() const override;

    virtual VkPipelineCacheCreateFlags getFlags() const override;

    virtual size_t getInitialDataSize() const override;

    virtual const void* getInitialData() const override;

    virtual const VkPipelineCache getPipelineCache() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_PIPELINECACHE_HPP_ */
