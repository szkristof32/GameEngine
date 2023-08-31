#include "pch.h"
#include "Descriptors.h"

#include "WhizzEngine/Rendering/RendererAPI.h"
#include "Platform/Vulkan/VulkanDescriptors.h"

namespace WhizzEngine {

	std::shared_ptr<DescriptorSetLayout> DescriptorSetLayout::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::None:		WZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	WZ_CORE_ASSERT(false, "RendererAPI::OpenGL is currently not supported!"); return nullptr;
			case RendererAPI::Vulkan:	return std::make_shared<VulkanDescriptorSetLayout>();
		}

		WZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<DescriptorPool> DescriptorPool::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::None:		WZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	WZ_CORE_ASSERT(false, "RendererAPI::OpenGL is currently not supported!"); return nullptr;
			case RendererAPI::Vulkan:	return std::make_shared<VulkanDescriptorPool>();
		}

		WZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}