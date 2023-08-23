#include "pch.h"
#include "Swapchain.h"

#include "WhizzEngine/Rendering/RendererAPI.h"
#include "Platform/Vulkan/VulkanSwapchain.h"

namespace WhizzEngine {

	std::shared_ptr<WhizzEngine::Swapchain> Swapchain::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::None:		WZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	WZ_CORE_WARN("RendererAPI::OpenGL does not support swapchains."); return nullptr;
			case RendererAPI::Vulkan:	return std::make_shared<VulkanSwapchain>();
		}

		WZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<WhizzEngine::Swapchain> Swapchain::Create(std::shared_ptr<Swapchain> previous)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::None:		WZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	WZ_CORE_WARN("RendererAPI::OpenGL does not support swapchains."); return nullptr;
			case RendererAPI::Vulkan:	return std::make_shared<VulkanSwapchain>(previous);
		}

		WZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}