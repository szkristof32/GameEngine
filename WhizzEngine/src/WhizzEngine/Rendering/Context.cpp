#include "pch.h"
#include "Context.h"

#include "WhizzEngine/Rendering/RendererAPI.h"

#if defined(WZ_PLATFORM_WINDOWS)
#include "Platform/OpenGL/WGLContext.h"
#include "Platform/Vulkan/VulkanContext.h"
#endif

namespace WhizzEngine {

	std::shared_ptr<GraphicsContext> GraphicsContext::Create(const void* windowHandle)
	{
#if defined(WZ_PLATFORM_WINDOWS)
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::None:		WZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return std::make_shared<WGLContext>((HWND)windowHandle);
			case RendererAPI::Vulkan:	return std::make_shared<VulkanContext>(windowHandle);
		}

		WZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
#else
		#error Unknown platform!
		return nullptr;
#endif
	}

}