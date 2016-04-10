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

#include "../visual/fn_visual_android_internal.hpp"

#include "fn_file_internal.hpp"

#include <sys/stat.h>

namespace vkts
{

VkBool32 VKTS_APIENTRY _filePrepareLoadBinary(const char* filename)
{
	auto androidApp = _visualGetAndroidApp();

	if (!androidApp)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "File: No android application.");

		return VK_FALSE;
	}

	AAssetManager* assetManager = androidApp->activity->assetManager;

    AAsset* sourceAsset = AAssetManager_open(assetManager, filename, AASSET_MODE_BUFFER);

    if (!sourceAsset)
    {
		vkts::logPrint(VKTS_LOG_ERROR, "File: Source asset not found: '%s'", filename);

		return VK_FALSE;
    }

    const void* sourceData = AAsset_getBuffer(sourceAsset);

	const off_t sourceLength = AAsset_getLength(sourceAsset);

	//

	std::string targetDirectory = std::string(_fileGetBaseDirectory());

	std::string targetFile = targetDirectory + std::string(filename);

	//

	std::string foldersToCreate = std::string(filename);

	auto lastSlash = foldersToCreate.find('/');

	while (lastSlash != foldersToCreate.npos)
	{
		targetDirectory += foldersToCreate.substr(0, lastSlash) + "/";

		mkdir(targetDirectory.c_str(), S_IXUSR | S_IXGRP);

		foldersToCreate = foldersToCreate.substr(lastSlash + 1);

		lastSlash = foldersToCreate.find('/');
	}

	//

	FILE* targetAsset = fopen(targetFile.c_str(), "wb");

	if (!targetAsset)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "File: Could not open target asset: '%s'", targetFile.c_str());

		return VK_FALSE;
	}

	auto targetLength = fwrite(sourceData, sizeof(char), (size_t)sourceLength, targetAsset);

	if (sourceLength != targetLength)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "File: Could not write target asset: '%s'", targetFile.c_str());

		return VK_FALSE;
	}

	fclose(targetAsset);

	//

	AAsset_close(sourceAsset);

	return VK_TRUE;
}


VkBool32 VKTS_APIENTRY _filePrepareSaveBinary(const char* filename)
{
	auto androidApp = _visualGetAndroidApp();

	if (!androidApp)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "File: No android application.");

		return VK_FALSE;
	}

	//

	std::string targetDirectory = std::string(_fileGetBaseDirectory());

	//

	std::string foldersToCreate = std::string(filename);

	auto lastSlash = foldersToCreate.find('/');

	while (lastSlash != foldersToCreate.npos)
	{
		targetDirectory += foldersToCreate.substr(0, lastSlash) + "/";

		mkdir(targetDirectory.c_str(), S_IXUSR | S_IXGRP);

		foldersToCreate = foldersToCreate.substr(lastSlash + 1);

		lastSlash = foldersToCreate.find('/');
	}

	return VK_TRUE;
}

}
