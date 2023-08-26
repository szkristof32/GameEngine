#pragma once

#include "WhizzEngine/Rendering/Buffers.h"

namespace WhizzEngine {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer();
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AllocateData(uint32_t size) override;
		virtual void SetData(uint32_t size, const void* data, uint32_t offset = 0) override;
		virtual uint32_t GetDataSize() const override { return m_DataSize; }
	private:
		uint32_t m_RendererID;
		uint32_t m_DataSize = 0;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer();
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AllocateData(uint32_t size) override;
		virtual void SetData(uint32_t size, const void* data, uint32_t offset = 0) override;
		virtual uint32_t GetDataSize() const override { return m_DataSize; }
	private:
		uint32_t m_RendererID;
		uint32_t m_DataSize = 0;
	};

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer();

		virtual void SetData(uint32_t size, const void* data, uint32_t offset = 0) override;
	private:
		uint32_t m_RendererID;
	};

}