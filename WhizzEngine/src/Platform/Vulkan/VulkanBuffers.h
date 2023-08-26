#pragma once

#include "WhizzEngine/Rendering/Buffers.h"

#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

namespace WhizzEngine {

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer();
		virtual ~VulkanVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AllocateData(uint32_t size) override;
		virtual void SetData(uint32_t size, const void* data, uint32_t offset = 0) override;
		virtual uint32_t GetDataSize() const override { return m_DataSize; }
	private:
		void Delete();
	private:
		VkBuffer m_Buffer = nullptr;
		VmaAllocation m_Memory = nullptr;
		uint32_t m_DataSize = 0;

		VkBufferUsageFlags m_Usage;
		VmaMemoryUsage m_MemoryUsage;
	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer();
		virtual ~VulkanIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AllocateData(uint32_t size) override;
		virtual void SetData(uint32_t size, const void* data, uint32_t offset = 0) override;
		virtual uint32_t GetDataSize() const override { return m_DataSize; }
	private:
		void Delete();
	private:
		VkBuffer m_Buffer = nullptr;
		VmaAllocation m_Memory = 0;
		uint32_t m_DataSize = 0;

		VkBufferUsageFlags m_Usage;
		VmaMemoryUsage m_MemoryUsage;
	};

	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~VulkanUniformBuffer();

		virtual void SetData(uint32_t size, const void* data, uint32_t offset = 0) override;
		VkDescriptorBufferInfo DescriptorInfo(VkDeviceSize size, VkDeviceSize offset) { return { m_Buffer, offset, size }; }
	private:
		VkBuffer m_Buffer;
		VmaAllocation m_Memory;
	};

}