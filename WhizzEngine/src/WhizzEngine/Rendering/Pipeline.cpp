#include "pch.h"
#include "Pipeline.h"

#include "WhizzEngine/Rendering/RendererAPI.h"

#include "Platform/OpenGL/OpenGLPipeline.h"
#include "Platform/Vulkan/VulkanPipeline.h"

namespace WhizzEngine {

	std::shared_ptr<Pipeline> Pipeline::Create(const PipelineInfo& pipelineInfo)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::None:		WZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return std::make_shared<OpenGLPipeline>(pipelineInfo);
			case RendererAPI::Vulkan:	return std::make_shared<VulkanPipeline>(pipelineInfo);
		}

		WZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}