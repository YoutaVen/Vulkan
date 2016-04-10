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

#ifndef VKTS_DISPLACE_HPP_
#define VKTS_DISPLACE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class displace : public overwrite
{

public:

	displace() :
		overwrite()
    {
    }

    virtual ~displace()
    {
    }

    virtual VkBool32 meshBindDrawIndexedRecursive(const IMesh& mesh, const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const uint32_t bufferIndex) const
    {
    	IGraphicsPipelineSP graphicsPipeline;

    	for (size_t i = 0; i < allGraphicsPipelines.size(); i++)
    	{
    		if (allGraphicsPipelines[i]->getTessellationState())
    		{
    			graphicsPipeline = allGraphicsPipelines[i];

    			break;
    		}
    	}

    	if (!graphicsPipeline.get())
    	{
            logPrint(VKTS_LOG_WARNING, "displace: No tessellation state found");

            return VK_FALSE;
    	}

    	const auto& displace = mesh.getDisplace();

    	vkCmdPushConstants(cmdBuffer->getCommandBuffer(bufferIndex), graphicsPipeline->getLayout(), VK_SHADER_STAGE_GEOMETRY_BIT, 0, sizeof(float) * 2, glm::value_ptr(displace));

    	return VK_TRUE;
    }
};

} /* namespace vkts */

#endif /* VKTS_DISPLACE_HPP_ */
