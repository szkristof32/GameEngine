#pragma once

#include "WhizzEngine/Core/Core.h"
#include "WhizzEngine/Rendering/Attributes.h"
#include "WhizzEngine/Rendering/Buffers.h"

#include <memory>

namespace WhizzEngine {

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer, const AttributeLayout& attributeLayout) = 0;
		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) = 0;

		virtual std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() const = 0;
		virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const = 0;
		virtual const AttributeLayout& GetAttributeLayout() const = 0;

		WZ_CAST_TO(VertexArray);

		static std::shared_ptr<VertexArray> Create();
	};

}