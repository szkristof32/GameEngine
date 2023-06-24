#pragma once

#include "WhizzEngine/Core/Core.h"
#include "WhizzEngine/Rendering/Pipeline.h"
#include "WhizzEngine/Rendering/VertexArray.h"

#include <memory>

namespace WhizzEngine {

	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;

		virtual void Clear() const = 0;
		virtual void BindPipeline(std::shared_ptr<Pipeline> pipeline) = 0;
		virtual void Draw(uint32_t vertexCount) = 0;
		virtual void DrawIndexed(std::shared_ptr<VertexArray> vertexArray) = 0;

		WZ_CAST_TO(RendererAPI);

		static std::shared_ptr<RendererAPI> Create();
	};

}