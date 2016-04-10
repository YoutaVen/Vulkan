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

#ifndef VKTS_ICONTEXT_HPP_
#define VKTS_ICONTEXT_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class IContext: public IDestroyable
{

public:

    IContext() :
        IDestroyable()
    {
    }

    virtual ~IContext()
    {
    }

    virtual const IInitialResourcesSP& getInitialResources() const = 0;

    virtual const ICommandBuffersSP& getCommandBuffer() const = 0;

    virtual const VkSamplerCreateInfo& getSamplerCreateInfo() const = 0;

    virtual const VkImageViewCreateInfo& getImageViewCreateInfo() const = 0;

    virtual const IDescriptorSetLayoutSP& getDescriptorSetLayout() const = 0;

    //

    virtual IObjectSP useObject(const std::string& name) const = 0;

    virtual VkBool32 addObject(const IObjectSP& object) = 0;

    virtual VkBool32 removeObject(const IObjectSP& object) = 0;

    //

    virtual IMeshSP useMesh(const std::string& name) const = 0;

    virtual VkBool32 addMesh(const IMeshSP& mesh) = 0;

    virtual VkBool32 removeMesh(const IMeshSP& mesh) = 0;

    virtual ISubMeshSP useSubMesh(const std::string& name) const = 0;

    virtual VkBool32 addSubMesh(const ISubMeshSP& subMesh) = 0;

    virtual VkBool32 removeSubMesh(const ISubMeshSP& subMesh) = 0;

    //

    virtual IAnimationSP useAnimation(const std::string& name) const = 0;

    virtual VkBool32 addAnimation(const IAnimationSP& animation) = 0;

    virtual VkBool32 removeAnimation(const IAnimationSP& animation) = 0;

    virtual IChannelSP useChannel(const std::string& name) const = 0;

    virtual VkBool32 addChannel(const IChannelSP& channel) = 0;

    virtual VkBool32 removeChannel(const IChannelSP& channel) = 0;

    //

    virtual IBSDFMaterialSP useBSDFMaterial(const std::string& name) const = 0;

    virtual VkBool32 addBSDFMaterial(const IBSDFMaterialSP& material) = 0;

    virtual VkBool32 removeBSDFMaterial(const IBSDFMaterialSP& material) = 0;

    virtual IPhongMaterialSP usePhongMaterial(const std::string& name) const = 0;

    virtual VkBool32 addPhongMaterial(const IPhongMaterialSP& material) = 0;

    virtual VkBool32 removePhongMaterial(const IPhongMaterialSP& material) = 0;

    //

    virtual ITextureSP useTexture(const std::string& name) const = 0;

    virtual VkBool32 addTexture(const ITextureSP& texture) = 0;

    virtual VkBool32 removeTexture(const ITextureSP& texture) = 0;

    //

    virtual IMemoryImageSP useMemoryImage(const std::string& name) const = 0;

    virtual VkBool32 addMemoryImage(const IMemoryImageSP& memoryImage) = 0;

    virtual VkBool32 removeMemoryImage(const IMemoryImageSP& memoryImage) = 0;

    //

    virtual IImageDataSP useImageData(const std::string& name) const = 0;

    virtual VkBool32 addImageData(const IImageDataSP& imageData) = 0;

    virtual VkBool32 removeImageData(const IImageDataSP& imageData) = 0;

    //

    virtual void addStageImage(const IImageSP& stageImage) = 0;

    virtual void addStageBuffer(const IBufferSP& stageBuffer) = 0;

    virtual void addStageDeviceMemory(const IDeviceMemorySP& stageDeviceMemory) = 0;

};

typedef std::shared_ptr<IContext> IContextSP;

} /* namespace vkts */

#endif /* VKTS_ICONTEXT_HPP_ */
