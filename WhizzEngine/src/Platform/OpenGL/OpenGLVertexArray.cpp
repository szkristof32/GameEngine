#include "pch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace WhizzEngine {

	static GLenum ShaderDataTypeToOpenGLBaseType(DataType type)
	{
		switch (type)
		{
			case DataType::Float:    return GL_FLOAT;
			case DataType::Float2:   return GL_FLOAT;
			case DataType::Float3:   return GL_FLOAT;
			case DataType::Float4:   return GL_FLOAT;
			case DataType::Mat3:     return GL_FLOAT;
			case DataType::Mat4:     return GL_FLOAT;
			case DataType::Int:      return GL_INT;
			case DataType::Int2:     return GL_INT;
			case DataType::Int3:     return GL_INT;
			case DataType::Int4:     return GL_INT;
			case DataType::Bool:     return GL_BOOL;
		}

		WZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
		if (m_IndexBuffer)
			m_IndexBuffer->Bind();
		for (auto&& attribute : m_Attributes)
			glEnableVertexAttribArray(attribute.Location);
	}

	void OpenGLVertexArray::Unbind() const
	{
		for (auto&& attribute : m_Attributes)
			glDisableVertexAttribArray(attribute.Location);
		if (m_IndexBuffer)
			m_IndexBuffer->Unbind();
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer, const AttributeLayout& attributeLayout)
	{
		m_VertexBuffers.emplace_back(vertexBuffer);
		for (auto& attribute : attributeLayout)
		{
			glVertexAttribPointer(attribute.Location, attribute.ComponentCount, ShaderDataTypeToOpenGLBaseType(attribute.Type), attribute.Normalised, attributeLayout.GetStride(), (const void*)attribute.Offset);
		}
		m_Attributes.AddAttributes(attributeLayout);
	}

}