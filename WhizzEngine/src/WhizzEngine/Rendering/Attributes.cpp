#include "pch.h"
#include "Attributes.h"

namespace WhizzEngine {

	static uint32_t GetComponentCount(DataType type)
	{
		switch (type)
		{
			case DataType::Float:	return 1;
			case DataType::Float2:	return 2;
			case DataType::Float3:	return 3;
			case DataType::Float4:	return 4;
			case DataType::Int:		return 1;
			case DataType::Int2:	return 2;
			case DataType::Int3:	return 3;
			case DataType::Int4:	return 4;
			case DataType::Mat3:	return 3;
			case DataType::Mat4:	return 4;
			case DataType::Bool:	return 1;
		}

		WZ_CORE_ASSERT(false, "Invalid data type");
		return 0;
	}

	AttributeLayout::AttributeLayout(std::initializer_list<Attribute> attributes)
		: m_Attributes(attributes)
	{
		CalculateOffsetsAndStride();
	}

	void AttributeLayout::AddAttributes(const Attribute& attribute)
	{
		m_Attributes.emplace_back(attribute);
		CalculateOffsetsAndStride();
	}

	void AttributeLayout::AddAttributes(std::initializer_list<Attribute> attributes)
	{
		for (auto attribute : attributes)
			m_Attributes.emplace_back(attribute);
		CalculateOffsetsAndStride();
	}

	void AttributeLayout::AddAttributes(const AttributeLayout& attributeLayout)
	{
		for (auto attribute : attributeLayout)
			m_Attributes.emplace_back(attribute);
		CalculateOffsetsAndStride();
	}

	void AttributeLayout::CalculateOffsetsAndStride()
	{
		uint32_t offset = 0;
		for (uint32_t i = 0;i < m_Attributes.size();i++)
		{
			Attribute& attrib = m_Attributes[i];
			attrib.Location = i;
			attrib.Offset = offset;
			attrib.ComponentCount = GetComponentCount(attrib.Type);
			offset += DataTypeSize(attrib.Type);
		}
		m_Stride = offset;
	}

}