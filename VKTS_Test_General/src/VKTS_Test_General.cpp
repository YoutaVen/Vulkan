#include <vkts/vkts.hpp>

class Test : public vkts::IUpdateThread
{

private:

	std::string name;

	int32_t displayIndex;

	int32_t windowIndex;

public:

	Test(const std::string& name) :
			IUpdateThread(), name(name), displayIndex(-1), windowIndex(-1)
	{
	}

	virtual ~Test()
	{
	}

	virtual VkBool32 init(const vkts::IUpdateThreadContext& updateContext)
	{
		vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Initialize done.", name.c_str());

		vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Thread %d from %d.", name.c_str(), updateContext.getThreadIndex(), updateContext.getThreadCount());

		const auto displayIndexWalker = updateContext.getAttachedDisplayIndices().begin();

		const auto windowIndexWalker = updateContext.getAttachedWindowIndices().begin();

		if (displayIndexWalker != updateContext.getAttachedDisplayIndices().end() && windowIndexWalker != updateContext.getAttachedWindowIndices().end())
		{
			displayIndex = *displayIndexWalker;
			windowIndex = *windowIndexWalker;

			vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Found display index = %d", name.c_str(), displayIndex);
			vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Width x Height = %d x %d", name.c_str(), updateContext.getDisplayDimension(displayIndex).x, updateContext.getDisplayDimension(displayIndex).y);
			vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Found window index = %d", name.c_str(), windowIndex);
			vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Width x Height = %d x %d", name.c_str(), updateContext.getWindowDimension(windowIndex).x, updateContext.getWindowDimension(windowIndex).y);
		}

		return VK_TRUE;
	}

	virtual VkBool32 update(const vkts::IUpdateThreadContext& updateContext)
	{
		if (windowIndex >= 0)
		{
			vkts::logPrint(VKTS_LOG_DEBUG, "Test '%s': Width x Height = %d x %d", name.c_str(), updateContext.getWindowDimension(windowIndex).x, updateContext.getWindowDimension(windowIndex).y);

			if (updateContext.getKey(windowIndex, VKTS_KEY_Q))
			{
				vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Quit pressed!", name.c_str());

				return VK_FALSE;
			}
		}

		vkts::logPrint(VKTS_LOG_DEBUG, "Test '%s': Updating %f", name.c_str(), updateContext.getTotalTime());

		vkts::logPrint(VKTS_LOG_DEBUG, "Test '%s': Before barrier.", name.c_str());

		if (!vkts::barrierSync())
		{
			vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Barrier killed.", name.c_str());

			return VK_FALSE;
		}

		vkts::logPrint(VKTS_LOG_DEBUG, "Test '%s': After barrier.", name.c_str());

		if (name == "a" && updateContext.getTotalTime() > 10.0)
		{
			vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Time reached!", name.c_str());

			return VK_FALSE;
		}

		return VK_TRUE;
	}

	virtual void terminate(const vkts::IUpdateThreadContext& updateContext)
	{
		vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Terminate done.", name.c_str());
	}

};

int main()
{
	vkts::IUpdateThreadSP a = vkts::IUpdateThreadSP(new Test("a"));
	vkts::IUpdateThreadSP b = vkts::IUpdateThreadSP(new Test("b"));
	vkts::IUpdateThreadSP c = vkts::IUpdateThreadSP(new Test("c"));

	if (!a.get() || !b.get() || !b.get())
	{
		printf("Test: Could not create test application.");

		return -1;
	}

	vkts::engineInit();

	vkts::logSetLevel(VKTS_LOG_INFO);

	vkts::logPrint(VKTS_LOG_INFO, "Test: Number of processors = %u.", vkts::processorGetNumber());

	//
	// Visual setup.
	//

	// Note: If using VK_KHR_display, values have to be set.
	if (!vkts::visualInit(VK_NULL_HANDLE, VK_NULL_HANDLE))
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not initialize visual.");

		vkts::engineTerminate();

		return -1;
	}

	vkts::logPrint(VKTS_LOG_INFO, "Test: Number of displays = %d", vkts::visualGetNumberDisplays());

	auto display = vkts::visualCreateDefaultDisplay().lock();

	if (!display.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not create display.");

		vkts::visualTerminate();

		vkts::engineTerminate();

		return -1;
	}

	vkts::logPrint(VKTS_LOG_INFO, "Test: Display = %d Width x Height = %d x %d", display->getIndex(), display->getWidth(), display->getHeight());

	auto window = vkts::visualCreateWindow(display, "Test", 1024, 768, VK_FALSE, VK_TRUE, VK_FALSE).lock();

	if (!window.get())
	{
		window = vkts::visualGetWindow(VKTS_DEFAULT_WINDOW_INDEX).lock();

		if (!window.get())
		{
			vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not create window.");

			display->destroy();

			vkts::visualTerminate();

			vkts::engineTerminate();

			return -1;
		}
	}

	//
	// Engine setup.
	//

	vkts::engineAddUpdateThread(a);
	vkts::engineAddUpdateThread(b);
	vkts::engineAddUpdateThread(c);

	//
	// File test.
	//

	auto textBuffer = vkts::fileLoadText("test/general/input.txt");

	if (textBuffer.get())
	{
		if (!vkts::fileSaveText("test/general/output.txt", textBuffer))
		{
			vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not save file.");
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not load file.");
	}

	auto createdTextBuffer = vkts::textBufferCreate("");

	if (createdTextBuffer.get())
	{
		createdTextBuffer->puts("Created text by VKTS.");
		createdTextBuffer->puts("\nAppended text.");

		if (!vkts::fileSaveText("test/general/created.txt", createdTextBuffer))
		{
			vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not save created text file.");
		}

		createdTextBuffer->seek(0, VKTS_SEARCH_ABSOLUTE);

		char buffer[256];

		while (createdTextBuffer->gets(buffer, 256))
		{
			vkts::logPrint(VKTS_LOG_INFO, "Test: Created text line: '%s'", buffer);
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not create text.");
	}

	//
	// Image test.
	//

	auto imageTga = vkts::imageDataLoad("test/general/crate_input.tga");

	if (imageTga.get())
	{
		if (!vkts::imageDataSave("test/general/crate_output.tga", imageTga))
		{
			vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not save TGA image.");
		}

		auto mipMaps = vkts::imageDataMipmap(imageTga, VK_TRUE, "test/general/crate_output.tga");

		for (size_t i = 0; i < mipMaps.size(); i++)
		{
			if (!vkts::imageDataSave(mipMaps[i]->getName().c_str(), mipMaps[i]))
			{
				vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not save mip map TGA image.");
			}
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not load TGA image.");
	}

	//

	auto imageHdr = vkts::imageDataLoad("test/general/CraterLake_input.hdr");

	if (imageHdr.get())
	{
		if (!vkts::imageDataSave("test/general/CraterLake_output.hdr", imageHdr))
		{
			vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not save HDR image.");
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not load HDR image.");
	}

	//

	auto createdTga = vkts::imageDataCreate("test/general/created.tga", 16, 16, 1, 1.0f, 0.0f, 0.0f, 0.0f, VK_IMAGE_TYPE_2D, VK_FORMAT_R8G8B8_UNORM);

	if (createdTga.get())
	{
		if (!vkts::imageDataSave(createdTga->getName().c_str(), createdTga))
		{
			vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not save created TGA image.");
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not create TGA image.");
	}

	//

	auto createdHdr = vkts::imageDataCreate("test/general/created.hdr", 16, 16, 1, 1.0f, 0.0f, 0.0f, 0.0f, VK_IMAGE_TYPE_2D, VK_FORMAT_R32G32B32_SFLOAT);

	if (createdHdr.get())
	{
		if (!vkts::imageDataSave(createdHdr->getName().c_str(), createdHdr))
		{
			vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not save created HDR image.");
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not create HDR image.");
	}

	//

	auto imageRGB = vkts::imageDataLoad("test/general/parque_rgb.tga");

	if (imageRGB.get())
	{
		auto imageRGBA = vkts::imageDataConvert(imageRGB, VK_FORMAT_R8G8B8A8_UNORM, "test/general/parque_rgba.tga");

		if (imageRGBA.get())
		{
			if (!vkts::imageDataSave("test/general/parque_rgba.tga", imageRGBA))
			{
				vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not save converted RGBA image.");
			}
		}
		else
		{
			vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not convert RGB image.");
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not load RGB image.");
	}

	//
	// Execution.
	//

	vkts::engineRun();

	//
	// Termination.
	//

	window->destroy();

	display->destroy();

	vkts::visualTerminate();

	vkts::engineTerminate();

	return 0;
}
