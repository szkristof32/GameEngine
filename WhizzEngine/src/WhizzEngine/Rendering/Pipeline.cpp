#include "pch.h"
#include "Pipeline.h"

#include "Platform/OpenGL/OpenGLPipeline.h"

namespace WhizzEngine {

	std::shared_ptr<Pipeline> Pipeline::Create(const PipelineInfo& pipelineInfo)
	{
		return std::make_shared<OpenGLPipeline>(pipelineInfo);
	}

}