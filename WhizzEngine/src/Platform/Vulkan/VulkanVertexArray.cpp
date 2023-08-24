#include "pch.h"
#include "VulkanVertexArray.h"

namespace WhizzEngine {

	void VulkanVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer, const AttributeLayout& attributeLayout)
	{
		m_VertexBuffers.emplace_back(vertexBuffer);
		m_Attributes.AddAttributes(attributeLayout);
	}

}