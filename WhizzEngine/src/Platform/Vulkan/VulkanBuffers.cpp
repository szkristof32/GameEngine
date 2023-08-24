#include "pch.h"
#include "VulkanBuffers.h"

#include "WhizzEngine/Core/Engine.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace WhizzEngine {

	VulkanVertexBuffer::VulkanVertexBuffer()
		: m_Usage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT), m_MemoryUsage(VMA_MEMORY_USAGE_CPU_TO_GPU)
	{
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		Delete();
	}

	void VulkanVertexBuffer::Delete()
	{
		vmaDestroyBuffer(Engine::GetContext()->As<VulkanContext>(), m_Buffer, m_Memory);
		m_Buffer = nullptr;
	}

	void VulkanVertexBuffer::Bind() const
	{
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(nullptr, 0, 1, &m_Buffer, &offset); // TODO: command buffers
	}

	void VulkanVertexBuffer::Unbind() const
	{
	}

	void VulkanVertexBuffer::AllocateData(uint32_t size)
	{
		if (m_Buffer) Delete();

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = m_Usage;

		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = m_MemoryUsage;

		WZ_CORE_ASSERT(vmaCreateBuffer(Engine::GetContext()->As<VulkanContext>(), &bufferInfo, &allocInfo, &m_Buffer, &m_Memory, nullptr) == VK_SUCCESS, "Failed to create buffer!");
	}

	void VulkanVertexBuffer::SetData(uint32_t size, const void* data, uint32_t offset)
	{
		if (!m_Memory) AllocateData(size);

		auto& context = Engine::GetContext()->As<VulkanContext>();
		void* mapped;
		vmaMapMemory(context, m_Memory, &mapped);
		memcpy(mapped, data, size);
		vmaUnmapMemory(context, m_Memory);
		m_DataSize = size;
	}

	VulkanIndexBuffer::VulkanIndexBuffer()
		: m_Usage(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT), m_MemoryUsage(VMA_MEMORY_USAGE_CPU_TO_GPU)
	{
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		Delete();
	}

	void VulkanIndexBuffer::Delete()
	{
		vmaDestroyBuffer(Engine::GetContext()->As<VulkanContext>(), m_Buffer, m_Memory);
		m_Buffer = nullptr;
	}

	void VulkanIndexBuffer::Bind() const
	{
		VkDeviceSize offset = 0;
		vkCmdBindIndexBuffer(nullptr, m_Buffer, offset, VK_INDEX_TYPE_UINT32); // TODO: command buffers
	}

	void VulkanIndexBuffer::Unbind() const
	{
	}

	void VulkanIndexBuffer::AllocateData(uint32_t size)
	{
		if (m_Buffer) Delete();

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = m_Usage;

		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = m_MemoryUsage;

		WZ_CORE_ASSERT(vmaCreateBuffer(Engine::GetContext()->As<VulkanContext>(), &bufferInfo, &allocInfo, &m_Buffer, &m_Memory, nullptr) == VK_SUCCESS, "Failed to create buffer!");
	}

	void VulkanIndexBuffer::SetData(uint32_t size, const void* data, uint32_t offset)
	{
		if (!m_Memory) AllocateData(size);

		auto& context = Engine::GetContext()->As<VulkanContext>();
		void* mapped;
		vmaMapMemory(context, m_Memory, &mapped);
		memcpy(mapped, data, size);
		vmaUnmapMemory(context, m_Memory);
		m_DataSize = size;
	}

}