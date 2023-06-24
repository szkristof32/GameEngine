#pragma once

#include "WhizzEngine/Rendering/Pipeline.h"

namespace WhizzEngine {

	class OpenGLPipeline : public Pipeline
	{
	public:
		OpenGLPipeline(const PipelineInfo& pipelineInfo);
		~OpenGLPipeline();

		virtual std::shared_ptr<Shader> GetShader() const { return m_Shader; }
		virtual const AttributeLayout& GetAttributeLayout() const { return m_AttributeLayout; }
	private:
		std::shared_ptr<Shader> m_Shader;
		AttributeLayout m_AttributeLayout;
	};

}