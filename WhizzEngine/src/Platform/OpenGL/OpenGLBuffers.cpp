#include "pch.h"
#include "OpenGLBuffers.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

namespace WhizzEngine {

	OpenGLVertexBuffer::OpenGLVertexBuffer()
	{
		glCreateBuffers(1, &m_RendererID);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::AllocateData(uint32_t size)
	{
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		m_DataSize = size;
	}

	void OpenGLVertexBuffer::SetData(uint32_t size, const void* data, uint32_t offset)
	{
		if (size > m_DataSize) AllocateData(size);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer()
	{
		glCreateBuffers(1, &m_RendererID);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OpenGLIndexBuffer::AllocateData(uint32_t size)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		m_DataSize = size;
	}

	void OpenGLIndexBuffer::SetData(uint32_t size, const void* data, uint32_t offset)
	{
		if (size > m_DataSize) AllocateData(size);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLUniformBuffer::SetData(uint32_t size, const void* data, uint32_t offset)
	{
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}

}