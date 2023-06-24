#pragma once

#include "WhizzEngine/Rendering/VertexArray.h"

#include <vector>

namespace WhizzEngine {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer, const AttributeLayout& attributeLayout) override;
		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override { m_IndexBuffer = indexBuffer; }

		virtual std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() const { return m_VertexBuffers; }
		virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }
		virtual const AttributeLayout& GetAttributeLayout() const override { return m_Attributes; }
	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
		AttributeLayout m_Attributes;
	};

}