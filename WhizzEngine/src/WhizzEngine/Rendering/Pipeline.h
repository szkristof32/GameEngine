#pragma once

#include "WhizzEngine/Core/Core.h"
#include "WhizzEngine/Rendering/Shader.h"
#include "WhizzEngine/Rendering/Attributes.h"

#include <string>
#include <memory>

namespace WhizzEngine {

	struct PipelineInfo
	{
		std::string ShaderPath;
		AttributeLayout AttribLayout;
	};

	class Pipeline
	{
	public:
		virtual ~Pipeline() = default;

		WZ_CAST_TO(Pipeline);

		virtual std::shared_ptr<Shader> GetShader() const = 0;
		virtual const AttributeLayout& GetAttributeLayout() const = 0;

		static std::shared_ptr<Pipeline> Create(const PipelineInfo& pipelineInfo);
	};

}