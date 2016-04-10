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

#ifndef VKTS_CONTEXT_HPP_
#define VKTS_CONTEXT_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Context: public IContext
{

private:

    VkBool32 replace;

    const IInitialResourcesSP initialResources;

    const ICommandBuffersSP cmdBuffer;

    const VkSamplerCreateInfo samplerCreateInfo;

    const VkImageViewCreateInfo imageViewCreateInfo;

    const IDescriptorSetLayoutSP descriptorSetLayout;

    SmartPointerMap<std::string, IObjectSP> allObjects;
    mutable std::vector<std::string> allUsedObjects;

    SmartPointerMap<std::string, IMeshSP> allMeshes;

    SmartPointerMap<std::string, ISubMeshSP> allSubMeshes;

    SmartPointerMap<std::string, IAnimationSP> allAnimations;

    SmartPointerMap<std::string, IChannelSP> allChannels;

    SmartPointerMap<std::string, IBSDFMaterialSP> allBSDFMaterials;
    mutable std::vector<std::string> allUsedBSDFMaterials;

    SmartPointerMap<std::string, IPhongMaterialSP> allPhongMaterials;
    mutable std::vector<std::string> allUsedPhongMaterials;

    SmartPointerMap<std::string, ITextureSP> allTextures;

    SmartPointerMap<std::string, IMemoryImageSP> allMemoryImages;

    SmartPointerMap<std::string, IImageDataSP> allImageDatas;

    //

    SmartPointerVector<IImageSP> allStageImages;
    SmartPointerVector<IBufferSP> allStageBuffers;
    SmartPointerVector<IDeviceMemorySP> allStageDeviceMemories;

    template<class S, class T>
    const T& get(const S& key, const SmartPointerMap<S, T>& map) const
    {
        static T empty;

        try
        {
            return map[key];
        }
        catch (const std::out_of_range& outOfRange)
        {
            logPrint(VKTS_LOG_SEVERE, "Context: Out of Range error '%s'", outOfRange.what());
        }

        return empty;
    }

    template<class S, class T>
    VkBool32 add(const S& key, const T& value, SmartPointerMap<S, T>& map)
    {
        if (replace || map.find(key) == map.size())
        {
            map[key] = value;

            return VK_TRUE;
        }

        return VK_FALSE;
    }

    template<class S, class T>
    VkBool32 remove(const S& key, SmartPointerMap<S, T>& map)
    {
        auto index = map.find(key);

        if (index != map.size())
        {
            map.removeAt(index);

            return VK_TRUE;
        }

        return VK_FALSE;
    }

public:

    Context() = delete;
    Context(const VkBool32 replace, const IInitialResourcesSP& initialResources, const ICommandBuffersSP& cmdBuffer, const VkSamplerCreateInfo& samplerCreateInfo, const VkImageViewCreateInfo& imageViewCreateInfo, const IDescriptorSetLayoutSP& descriptorSetLayout);
    Context(const Context& other) = delete;
    Context(Context&& other) = delete;
    virtual ~Context();

    Context& operator =(const Context& other) = delete;

    Context& operator =(Context && other) = delete;

    //
    // IContext
    //

    virtual const IInitialResourcesSP& getInitialResources() const override;

    virtual const ICommandBuffersSP& getCommandBuffer() const override;

    virtual const VkSamplerCreateInfo& getSamplerCreateInfo() const override;

    virtual const VkImageViewCreateInfo& getImageViewCreateInfo() const override;

    virtual const IDescriptorSetLayoutSP& getDescriptorSetLayout() const override;

    //

    virtual IObjectSP useObject(const std::string& name) const override;

    virtual VkBool32 addObject(const IObjectSP& object) override;

    virtual VkBool32 removeObject(const IObjectSP& object) override;

    //

    virtual IMeshSP useMesh(const std::string& name) const override;

    virtual VkBool32 addMesh(const IMeshSP& mesh) override;

    virtual VkBool32 removeMesh(const IMeshSP& mesh) override;

    virtual ISubMeshSP useSubMesh(const std::string& name) const override;

    virtual VkBool32 addSubMesh(const ISubMeshSP& subMesh) override;

    virtual VkBool32 removeSubMesh(const ISubMeshSP& subMesh) override;

    //

    virtual IAnimationSP useAnimation(const std::string& name) const override;

    virtual VkBool32 addAnimation(const IAnimationSP& animation) override;

    virtual VkBool32 removeAnimation(const IAnimationSP& animation) override;

    virtual IChannelSP useChannel(const std::string& name) const override;

    virtual VkBool32 addChannel(const IChannelSP& channel) override;

    virtual VkBool32 removeChannel(const IChannelSP& channel) override;

    //

    virtual IBSDFMaterialSP useBSDFMaterial(const std::string& name) const override;

    virtual VkBool32 addBSDFMaterial(const IBSDFMaterialSP& material) override;

    virtual VkBool32 removeBSDFMaterial(const IBSDFMaterialSP& material) override;

    virtual IPhongMaterialSP usePhongMaterial(const std::string& name) const override;

    virtual VkBool32 addPhongMaterial(const IPhongMaterialSP& material) override;

    virtual VkBool32 removePhongMaterial(const IPhongMaterialSP& material) override;

    //

    virtual ITextureSP useTexture(const std::string& name) const override;

    virtual VkBool32 addTexture(const ITextureSP& texture) override;

    virtual VkBool32 removeTexture(const ITextureSP& texture) override;

    //

    virtual IMemoryImageSP useMemoryImage(const std::string& name) const override;

    virtual VkBool32 addMemoryImage(const IMemoryImageSP& memoryImage) override;

    virtual VkBool32 removeMemoryImage(const IMemoryImageSP& memoryImage) override;

    //

    virtual IImageDataSP useImageData(const std::string& name) const override;

    virtual VkBool32 addImageData(const IImageDataSP& imageData) override;

    virtual VkBool32 removeImageData(const IImageDataSP& imageData) override;

    //

    virtual void addStageImage(const IImageSP& stageImage) override;

    virtual void addStageBuffer(const IBufferSP& stageBuffer) override;

    virtual void addStageDeviceMemory(const IDeviceMemorySP& stageDeviceMemory) override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_CONTEXT_HPP_ */
