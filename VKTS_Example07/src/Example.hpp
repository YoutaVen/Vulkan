/**
 * VKTS Examples - Examples for Vulkan using VulKan ToolS.
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

#ifndef EXAMPLE_HPP_
#define EXAMPLE_HPP_

#include <vkts/vkts.hpp>

#include "BuildCommandTask.hpp"

#define VKTS_EXAMPLE_NAME "Example07"

#define VKTS_NUMBER_DYNAMIC_STATES 2

#define VKTS_NUMBER_BUFFERS 2
#define VKTS_SHADER_STAGE_COUNT 5
#define VKTS_PIPELINE_CACHE_SIZE 0

#define VKTS_BINDING_VERTEX_BUFFER 0

#define VKTS_VERTEX_SHADER_NAME "shader/SPIR/V/phong_displace.vert.spv"
#define VKTS_TESSELLATION_CONTROL_SHADER_NAME "shader/SPIR/V/phong_displace.tesc.spv"
#define VKTS_TESSELLATION_EVALUATION_SHADER_NAME "shader/SPIR/V/phong_displace.tese.spv"
#define VKTS_GEOMETRY_SHADER_NAME "shader/SPIR/V/phong_displace.geom.spv"
#define VKTS_FRAGMENT_SHADER_NAME "shader/SPIR/V/phong_displace.frag.spv"

#define VKTS_SCENE_NAME "valley_terrain/valley_terrain.vkts"

#define VKTS_NUMBER_TASKS 8

class Example: public vkts::IUpdateThread
{

private:

	const vkts::IInitialResourcesSP initialResources;

	const int32_t windowIndex;

	const vkts::ISurfaceSP surface;

	glm::uvec2 windowDimension;

	vkts::ICameraSP camera;
	vkts::IInputControllerSP inputController;

	vkts::SmartPointerVector<vkts::IUpdateableSP> allUpdateables;

	vkts::ICommandPoolSP commandPool;

    vkts::ISemaphoreSP imageAcquiredSemaphore;
    vkts::ISemaphoreSP renderingCompleteSemaphore;

	vkts::IDescriptorSetLayoutSP descriptorSetLayout;
    VkDescriptorBufferInfo descriptorBufferInfos[1 + 1];
    VkWriteDescriptorSet writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM];

	vkts::IBufferObjectSP vertexViewProjectionUniformBuffer;
	vkts::IBufferObjectSP fragmentUniformBuffer;

	vkts::IShaderModuleSP vertexShaderModule;
	vkts::IShaderModuleSP tessellationControlShaderModule;
	vkts::IShaderModuleSP tessellationEvaluationShaderModule;
	vkts::IShaderModuleSP geometryShaderModule;
	vkts::IShaderModuleSP fragmentShaderModule;

	vkts::IPipelineLayoutSP pipelineLayout;

	vkts::IContextSP sceneContext;
	vkts::ISceneSP scene;

	vkts::SmartPointerVector<IBuildCommandTaskSP> allBuildCommandTasks;

	vkts::ISwapchainSP swapchain;

	vkts::IRenderPassSP renderPass;

	vkts::SmartPointerVector<vkts::IGraphicsPipelineSP> allGraphicsPipelines;

	vkts::IMemoryImageSP depthTexture;
	vkts::IImageViewSP depthStencilImageView;

    uint32_t swapchainImagesCount;

    vkts::SmartPointerVector<vkts::IImageViewSP> swapchainImageView;

    vkts::SmartPointerVector<vkts::IFramebufferSP> framebuffer;

    vkts::SmartPointerVector<vkts::ICommandBuffersSP> cmdBuffer;

    uint32_t commandBufferCount;

    VkCommandBuffer secondaryCmdBuffers[VKTS_NUMBER_TASKS];

	VkBool32 buildCmdBuffer(const int32_t usedBuffer);

	VkBool32 buildFramebuffer(const int32_t usedBuffer);

	VkBool32 buildSwapchainImageView(const int32_t usedBuffer);

	VkBool32 updateDescriptorSets();

	VkBool32 buildScene(const vkts::IUpdateThreadContext& updateContext, const vkts::ICommandBuffersSP& cmdBuffer);

	VkBool32 buildDepthStencilImageView();

	VkBool32 buildDepthTexture(const vkts::ICommandBuffersSP& cmdBuffer);

	VkBool32 buildPipeline();

	VkBool32 buildRenderPass();

	VkBool32 buildPipelineLayout();

	VkBool32 buildDescriptorSetLayout();

	VkBool32 buildShader();

	VkBool32 buildUniformBuffers();

	VkBool32 buildResources(const vkts::IUpdateThreadContext& updateContext);

	void terminateResources(const vkts::IUpdateThreadContext& updateContext);

public:

	Example(const vkts::IInitialResourcesSP& initialResources, const int32_t windowIndex, const vkts::ISurfaceSP& surface);

	virtual ~Example();

	//
	// Vulkan initialization.
	//
	virtual VkBool32 init(const vkts::IUpdateThreadContext& updateContext);

	//
	// Vulkan update.
	//
	virtual VkBool32 update(const vkts::IUpdateThreadContext& updateContext);

	//
	// Vulkan termination.
	//
	virtual void terminate(const vkts::IUpdateThreadContext& updateContext);

};

#endif /* EXAMPLE_HPP_ */
