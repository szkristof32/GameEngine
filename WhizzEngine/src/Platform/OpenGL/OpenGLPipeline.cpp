#include "pch.h"
#include "OpenGLPipeline.h"

#include <glad/glad.h>

namespace WhizzEngine {

	OpenGLPipeline::OpenGLPipeline(const PipelineInfo& pipelineInfo)
		: m_AttributeLayout(pipelineInfo.AttribLayout)
	{
		m_Shader = Shader::Create(pipelineInfo.ShaderPath);
	}

	OpenGLPipeline::~OpenGLPipeline()
	{
	}

}