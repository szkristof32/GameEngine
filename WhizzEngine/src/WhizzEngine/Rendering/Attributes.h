#pragma once

#include "WhizzEngine/Core/Assert.h"

#include <vector>

namespace WhizzEngine {

	enum class DataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Mat3, Mat4, Bool
	};

	static uint32_t DataTypeSize(DataType type)
	{
		switch (type)
		{
			case DataType::Float:    return 4;
			case DataType::Float2:   return 4 * 2;
			case DataType::Float3:   return 4 * 3;
			case DataType::Float4:   return 4 * 4;
			case DataType::Mat3:     return 4 * 3 * 3;
			case DataType::Mat4:     return 4 * 4 * 4;
			case DataType::Int:      return 4;
			case DataType::Int2:     return 4 * 2;
			case DataType::Int3:     return 4 * 3;
			case DataType::Int4:     return 4 * 4;
			case DataType::Bool:     return 1;
		}

		WZ_CORE_ASSERT(false, "Invalid data type!");
		return 0;
	}

	struct Attribute
	{
		DataType Type;
		bool Normalised;
		uint32_t Location = 0;
		uint32_t ComponentCount = 1;
		uint32_t Offset = 0;

		Attribute(DataType type, bool normalised = false)
			: Type(type), Normalised(normalised)
		{}

		bool operator==(const Attribute& other) const
		{
			return Type == other.Type && Location == other.Location;
		}
	};

	class AttributeLayout
	{
	public:
		AttributeLayout(std::initializer_list<Attribute> attributes);
		AttributeLayout() = default;

		void AddAttributes(const Attribute& attribute);
		void AddAttributes(std::initializer_list<Attribute> attributes);
		void AddAttributes(const AttributeLayout& attributeLayout);

		uint32_t GetStride() const { return m_Stride; }

		bool operator==(const AttributeLayout& other) const
		{
			return m_Attributes == other.m_Attributes;
		}

		std::vector<Attribute>::iterator begin() { return m_Attributes.begin(); }
		std::vector<Attribute>::iterator end() { return m_Attributes.end(); }
		std::vector<Attribute>::const_iterator begin() const { return m_Attributes.begin(); }
		std::vector<Attribute>::const_iterator end() const { return m_Attributes.end(); }
	private:
		void CalculateOffsetsAndStride();
	private:
		std::vector<Attribute> m_Attributes;
		uint32_t m_Stride = 0;
	};

}