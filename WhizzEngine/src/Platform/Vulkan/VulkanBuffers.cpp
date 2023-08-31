#include "pch.h"
#include "VulkanBuffers.h"

#include "WhizzEngine/Core/Application.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"

namespace WhizzEngine {

	std::shared_ptr<DescriptorPool> VulkanUniformBuffer::s_UniformDescriptorPool = nullptr;

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
		vmaDestroyBuffer(Application::Get().GetContext()->As<VulkanContext>(), m_Buffer, m_Memory);
		m_Buffer = nullptr;
	}

	void VulkanVertexBuffer::Bind() const
	{
		if (!m_Buffer) return;
		VkDeviceSize offset = 0;
		auto& cmdBuffer = Application::Get().GetRendererAPI()->As<VulkanRendererAPI>().GetCurrentCommandBuffer();
		vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &m_Buffer, &offset);
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

		WZ_CORE_ASSERT(vmaCreateBuffer(Application::Get().GetContext()->As<VulkanContext>(), &bufferInfo, &allocInfo, &m_Buffer, &m_Memory, nullptr) == VK_SUCCESS, "Failed to create buffer!");
	}

	void VulkanVertexBuffer::SetData(uint32_t size, const void* data, uint32_t offset)
	{
		if (!m_Memory) AllocateData(size);

		auto& context = Application::Get().GetContext()->As<VulkanContext>();
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
		vmaDestroyBuffer(Application::Get().GetContext()->As<VulkanContext>(), m_Buffer, m_Memory);
		m_Buffer = nullptr;
	}

	void VulkanIndexBuffer::Bind() const
	{
		if (!m_Buffer) return;
		VkDeviceSize offset = 0;
		auto& cmdBuffer = Application::Get().GetRendererAPI()->As<VulkanRendererAPI>().GetCurrentCommandBuffer();
		vkCmdBindIndexBuffer(cmdBuffer, m_Buffer, offset, VK_INDEX_TYPE_UINT32);
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

		WZ_CORE_ASSERT(vmaCreateBuffer(Application::Get().GetContext()->As<VulkanContext>(), &bufferInfo, &allocInfo, &m_Buffer, &m_Memory, nullptr) == VK_SUCCESS, "Failed to create buffer!");
	}

	void VulkanIndexBuffer::SetData(uint32_t size, const void* data, uint32_t offset)
	{
		if (!m_Memory) AllocateData(size);

		auto& context = Application::Get().GetContext()->As<VulkanContext>();
		void* mapped;
		vmaMapMemory(context, m_Memory, &mapped);
		memcpy(mapped, data, size);
		vmaUnmapMemory(context, m_Memory);
		m_DataSize = size;
	}

	VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size, uint32_t binding)
	{
		auto& context = Application::Get().GetContext()->As<VulkanContext>();
		auto& renderer = Application::Get().GetRendererAPI()->As<VulkanRendererAPI>();

		if (!s_UniformDescriptorPool)
		{
			s_UniformDescriptorPool = DescriptorPool::Create();
			s_UniformDescriptorPool->AddPoolType(DescriptorType::UniformBuffer, 64);
			s_UniformDescriptorPool->AddPoolType(DescriptorType::DynamicUniformBuffer, 32);
		}

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

		WZ_CORE_ASSERT(vmaCreateBuffer(context, &bufferInfo, &allocInfo, &m_Buffer, &m_Memory, nullptr) == VK_SUCCESS, "Failed to create buffer!");

		m_SetLayout = DescriptorSetLayout::Create();
		m_SetLayout->AddBinding(DescriptorType::UniformBuffer, ShaderStage::VertexShader | ShaderStage::FragmentShader);
		m_Descriptor = s_UniformDescriptorPool->AllocateDescriptor(m_SetLayout);
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		vmaDestroyBuffer(Application::Get().GetContext()->As<VulkanContext>(), m_Buffer, m_Memory);
	}

	void VulkanUniformBuffer::SetData(uint32_t size, const void* data, uint32_t offset)
	{
		auto& context = Application::Get().GetContext()->As<VulkanContext>();
		void* mapped;
		vmaMapMemory(context, m_Memory, &mapped);
		memcpy(mapped, data, size);
		vmaUnmapMemory(context, m_Memory);
	}

}