#include "pch.h"
#include "Buffers.h"

#include "WhizzEngine/Rendering/RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffers.h"
#include "Platform/Vulkan/VulkanBuffers.h"

namespace WhizzEngine {

	std::shared_ptr<VertexBuffer> VertexBuffer::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::None:		WZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return std::make_shared<OpenGLVertexBuffer>();
			case RendererAPI::Vulkan:	return std::make_shared<VulkanVertexBuffer>();
		}

		WZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::None:		WZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return std::make_shared<OpenGLIndexBuffer>();
			case RendererAPI::Vulkan:	return std::make_shared<VulkanIndexBuffer>();
		}

		WZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<WhizzEngine::UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::None:		WZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return std::make_shared<OpenGLUniformBuffer>(size, binding);
			case RendererAPI::Vulkan:	return std::make_shared<VulkanUniformBuffer>(size, binding);
		}

		WZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}