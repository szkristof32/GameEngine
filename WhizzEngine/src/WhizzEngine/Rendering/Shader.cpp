#include "pch.h"
#include "Shader.h"

#include "WhizzEngine/Rendering/RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/Vulkan/VulkanShader.h"

namespace WhizzEngine {

	std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::None:		WZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return std::make_shared<OpenGLShader>(filepath);
			case RendererAPI::Vulkan:	return std::make_shared<VulkanShader>(filepath);
		}

		WZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}