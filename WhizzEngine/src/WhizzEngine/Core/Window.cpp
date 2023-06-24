#include "pch.h"
#include "Window.h"

#if defined(WZ_PLATFORM_WINDOWS)
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace WhizzEngine {

	std::shared_ptr<Window> Window::Create(uint32_t width, uint32_t height, const std::string& title)
	{
#if defined(WZ_PLATFORM_WINDOWS)
		return std::make_shared<WindowsWindow>(width, height, title);
#else
		#error Unknown platform!
		return nullptr;
#endif
	}

}