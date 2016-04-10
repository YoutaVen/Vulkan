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

#include "Context.hpp"

namespace vkts
{

Context::Context(const VkBool32 replace, const IInitialResourcesSP& initialResources, const ICommandBuffersSP& cmdBuffer, const VkSamplerCreateInfo& samplerCreateInfo, const VkImageViewCreateInfo& imageViewCreateInfo, const IDescriptorSetLayoutSP& descriptorSetLayout) :
    IContext(), replace(replace), initialResources(initialResources), cmdBuffer(cmdBuffer), samplerCreateInfo(samplerCreateInfo), imageViewCreateInfo(imageViewCreateInfo), descriptorSetLayout(descriptorSetLayout), allObjects(), allUsedObjects(), allMeshes(), allSubMeshes(), allAnimations(), allChannels(), allBSDFMaterials(), allUsedBSDFMaterials(), allPhongMaterials(), allUsedPhongMaterials(), allTextures(), allImageDatas()
{
}

Context::~Context()
{
    destroy();
}

//
// IContext
//

const IInitialResourcesSP& Context::getInitialResources() const
{
    return initialResources;
}

const ICommandBuffersSP& Context::getCommandBuffer() const
{
    return cmdBuffer;
}

const VkSamplerCreateInfo& Context::getSamplerCreateInfo() const
{
    return samplerCreateInfo;
}

const VkImageViewCreateInfo& Context::getImageViewCreateInfo() const
{
    return imageViewCreateInfo;
}

const IDescriptorSetLayoutSP& Context::getDescriptorSetLayout() const
{
    return descriptorSetLayout;
}

//

IObjectSP Context::useObject(const std::string& name) const
{
	IObjectSP result = get(name, allObjects);

	if (std::find(allUsedObjects.begin(), allUsedObjects.end(), name) != allUsedObjects.end())
	{
		result = result->clone();
	}
	else
	{
		allUsedObjects.push_back(name);
	}

    return result;
}

VkBool32 Context::addObject(const IObjectSP& object)
{
    if (!object.get())
    {
        return VK_FALSE;
    }

    return add(object->getName(), object, allObjects);
}

VkBool32 Context::removeObject(const IObjectSP& object)
{
    if (!object.get())
    {
        return VK_FALSE;
    }

    auto removeElement = std::find(allUsedObjects.begin(), allUsedObjects.end(), object->getName());
    if (removeElement != allUsedObjects.end())
    {
    	allUsedObjects.erase(removeElement);
    }

    return remove(object->getName(), allObjects);
}

//

IMeshSP Context::useMesh(const std::string& name) const
{
    return get(name, allMeshes);
}

VkBool32 Context::addMesh(const IMeshSP& mesh)
{
    if (!mesh.get())
    {
        return VK_FALSE;
    }

    return add(mesh->getName(), mesh, allMeshes);
}

VkBool32 Context::removeMesh(const IMeshSP& mesh)
{
    if (!mesh.get())
    {
        return VK_FALSE;
    }

    return remove(mesh->getName(), allMeshes);
}

ISubMeshSP Context::useSubMesh(const std::string& name) const
{
    return get(name, allSubMeshes);
}

VkBool32 Context::addSubMesh(const ISubMeshSP& subMesh)
{
    if (!subMesh.get())
    {
        return VK_FALSE;
    }

    return add(subMesh->getName(), subMesh, allSubMeshes);
}

VkBool32 Context::removeSubMesh(const ISubMeshSP& subMesh)
{
    if (!subMesh.get())
    {
        return VK_FALSE;
    }

    return remove(subMesh->getName(), allSubMeshes);
}

//

IAnimationSP Context::useAnimation(const std::string& name) const
{
    return get(name, allAnimations);
}

VkBool32 Context::addAnimation(const IAnimationSP& animation)
{
    if (!animation.get())
    {
        return VK_FALSE;
    }

    return add(animation->getName(), animation, allAnimations);
}

VkBool32 Context::removeAnimation(const IAnimationSP& animation)
{
    if (!animation.get())
    {
        return VK_FALSE;
    }

    return remove(animation->getName(), allAnimations);
}

IChannelSP Context::useChannel(const std::string& name) const
{
    return get(name, allChannels);
}

VkBool32 Context::addChannel(const IChannelSP& channel)
{
    if (!channel.get())
    {
        return VK_FALSE;
    }

    return add(channel->getName(), channel, allChannels);
}

VkBool32 Context::removeChannel(const IChannelSP& channel)
{
    if (!channel.get())
    {
        return VK_FALSE;
    }

    return remove(channel->getName(), allChannels);
}

//

IBSDFMaterialSP Context::useBSDFMaterial(const std::string& name) const
{
	IBSDFMaterialSP result = get(name, allBSDFMaterials);

	if (std::find(allUsedBSDFMaterials.begin(), allUsedBSDFMaterials.end(), name) != allUsedBSDFMaterials.end())
	{
		result = result->clone();
	}
	else
	{
		allUsedBSDFMaterials.push_back(name);
	}

    return result;
}

VkBool32 Context::addBSDFMaterial(const IBSDFMaterialSP& material)
{
    if (!material.get())
    {
        return VK_FALSE;
    }

    if (!material.get())
    {
        return VK_FALSE;
    }

    return add(material->getName(), material, allBSDFMaterials);
}

VkBool32 Context::removeBSDFMaterial(const IBSDFMaterialSP& material)
{
    if (!material.get())
    {
        return VK_FALSE;
    }

    auto removeElement = std::find(allUsedBSDFMaterials.begin(), allUsedBSDFMaterials.end(), material->getName());
    if (removeElement != allUsedBSDFMaterials.end())
    {
    	allUsedBSDFMaterials.erase(removeElement);
    }

    return remove(material->getName(), allBSDFMaterials);
}

IPhongMaterialSP Context::usePhongMaterial(const std::string& name) const
{
	IPhongMaterialSP result = get(name, allPhongMaterials);

	if (std::find(allUsedPhongMaterials.begin(), allUsedPhongMaterials.end(), name) != allUsedPhongMaterials.end())
	{
		result = result->clone();
	}
	else
	{
		allUsedPhongMaterials.push_back(name);
	}

    return result;
}

VkBool32 Context::addPhongMaterial(const IPhongMaterialSP& material)
{
    if (!material.get())
    {
        return VK_FALSE;
    }

    return add(material->getName(), material, allPhongMaterials);
}

VkBool32 Context::removePhongMaterial(const IPhongMaterialSP& material)
{
    if (!material.get())
    {
        return VK_FALSE;
    }

    auto removeElement = std::find(allUsedPhongMaterials.begin(), allUsedPhongMaterials.end(), material->getName());
    if (removeElement != allUsedPhongMaterials.end())
    {
    	allUsedPhongMaterials.erase(removeElement);
    }

    return remove(material->getName(), allPhongMaterials);
}

//

ITextureSP Context::useTexture(const std::string& name) const
{
    return get(name, allTextures);
}

VkBool32 Context::addTexture(const ITextureSP& texture)
{
    if (!texture.get())
    {
        return VK_FALSE;
    }

    return add(texture->getName(), texture, allTextures);
}

VkBool32 Context::removeTexture(const ITextureSP& texture)
{
    if (!texture.get())
    {
        return VK_FALSE;
    }

    return remove(texture->getName(), allTextures);
}

//

IMemoryImageSP Context::useMemoryImage(const std::string& name) const
{
    return get(name, allMemoryImages);
}

VkBool32 Context::addMemoryImage(const IMemoryImageSP& memoryImage)
{
    if (!memoryImage.get())
    {
        return VK_FALSE;
    }

    return add(memoryImage->getName(), memoryImage, allMemoryImages);
}

VkBool32 Context::removeMemoryImage(const IMemoryImageSP& memoryImage)
{
    if (!memoryImage.get())
    {
        return VK_FALSE;
    }

    return remove(memoryImage->getName(), allMemoryImages);
}

//

IImageDataSP Context::useImageData(const std::string& name) const
{
    return get(name, allImageDatas);
}

VkBool32 Context::addImageData(const IImageDataSP& imageData)
{
    if (!imageData.get())
    {
        return VK_FALSE;
    }

    return add(imageData->getName(), imageData, allImageDatas);
}

VkBool32 Context::removeImageData(const IImageDataSP& imageData)
{
    if (!imageData.get())
    {
        return VK_FALSE;
    }

    return remove(imageData->getName(), allImageDatas);
}

//

void Context::addStageImage(const IImageSP& stageImage)
{
    if (stageImage.get())
    {
        allStageImages.append(stageImage);
    }
}

void Context::addStageBuffer(const IBufferSP& stageBuffer)
{
    if (stageBuffer.get())
    {
        allStageBuffers.append(stageBuffer);
    }
}

void Context::addStageDeviceMemory(const IDeviceMemorySP& stageDeviceMemory)
{
    if (stageDeviceMemory.get())
    {
        allStageDeviceMemories.append(stageDeviceMemory);
    }
}

//
// IDestroyable
//

void Context::destroy()
{
    // Only free resources, but do not destroy them.

    allObjects.clear();

    allMeshes.clear();

    allSubMeshes.clear();

    allAnimations.clear();

    allChannels.clear();

    allBSDFMaterials.clear();

    allPhongMaterials.clear();

    allTextures.clear();

    allImageDatas.clear();

    // Stage resources have to be deleted.

    for (uint32_t i = 0; i < allStageImages.size(); i++)
    {
        if (allStageImages[i].get())
        {
            allStageImages[i]->destroy();
        }
    }
    allStageImages.clear();

    for (uint32_t i = 0; i < allStageBuffers.size(); i++)
    {
        if (allStageBuffers[i].get())
        {
            allStageBuffers[i]->destroy();
        }
    }
    allStageBuffers.clear();

    for (uint32_t i = 0; i < allStageDeviceMemories.size(); i++)
    {
        if (allStageDeviceMemories[i].get())
        {
            allStageDeviceMemories[i]->destroy();
        }
    }
    allStageDeviceMemories.clear();
}

} /* namespace vkts */
