#include "pch.h"
#include "Context.h"

#if defined(WZ_PLATFORM_WINDOWS)
#include "Platform/OpenGL/WGLContext.h"
#endif

namespace WhizzEngine {

	std::shared_ptr<GraphicsContext> GraphicsContext::Create(const void* windowHandle)
	{
#if defined(WZ_PLATFORM_WINDOWS)
		return std::make_shared<WGLContext>((HWND)windowHandle);
#else
		#error Unknown platform!
		return nullptr;
#endif
	}

}