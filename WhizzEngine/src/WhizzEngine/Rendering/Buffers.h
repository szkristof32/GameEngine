#pragma once

#include "WhizzEngine/Core/Core.h"
#include "WhizzEngine/Rendering/Shader.h"

#include <memory>

namespace WhizzEngine {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AllocateData(uint32_t size) = 0;
		virtual void SetData(uint32_t size, const void* data, uint32_t offset = 0) = 0;
		virtual uint32_t GetDataSize() const = 0;

		WZ_CAST_TO(VertexBuffer);

		static std::shared_ptr<VertexBuffer> Create();
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AllocateData(uint32_t size) = 0;
		virtual void SetData(uint32_t size, const void* data, uint32_t offset = 0) = 0;
		virtual uint32_t GetDataSize() const = 0;

		WZ_CAST_TO(IndexBuffer);

		static std::shared_ptr<IndexBuffer> Create();
	};

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void SetData(uint32_t size, const void* data, uint32_t offset = 0) = 0;

		WZ_CAST_TO(UniformBuffer);

		static std::shared_ptr<UniformBuffer> Create(uint32_t size, uint32_t binding);
	};

}