#pragma once

#include "WhizzEngine/Rendering/Pipeline.h"

#include <vulkan/vulkan.h>

namespace WhizzEngine {

	enum class DescriptorType
	{
		None = 0,
		UniformBuffer, DynamicUniformBuffer, ImageSampler
	};

	enum class ShaderStage
	{
		None = 0,
		VertexShader = 0x1,
		FragmentShader = 0x2,
		GeomertyShader = 0x4

	};
	
	constexpr int operator&(ShaderStage a, ShaderStage b)
	{
		return (int)a & (int)b;
	}
	constexpr ShaderStage operator|(ShaderStage a, ShaderStage b)
	{
		return (ShaderStage)((int)a | (int)b);
	}

	class DescriptorSet
	{
	public:
		virtual ~DescriptorSet() = default;

		WZ_CAST_TO(DescriptorSet);

		virtual void Bind(std::shared_ptr<Pipeline> pipeline, std::vector<uint32_t> dynamicOffsets = {}) = 0;
	};

	class DescriptorSetLayout
	{
	public:
		virtual ~DescriptorSetLayout() = default;

		WZ_CAST_TO(DescriptorSetLayout);

		virtual void AddBinding(DescriptorType type, ShaderStage stage) = 0;

		static std::shared_ptr<DescriptorSetLayout> Create();
	};

	class DescriptorPool
	{
	public:
		virtual ~DescriptorPool() = default;

		WZ_CAST_TO(DescriptorPool);

		virtual void AddPoolType(DescriptorType type, uint32_t count) = 0;

		virtual std::shared_ptr<DescriptorSet> AllocateDescriptor(std::shared_ptr<DescriptorSetLayout> layout) const = 0;
		virtual void FreeDescriptors(std::vector<DescriptorSet>& descriptors) const = 0;
		virtual void Reset() = 0;

		static std::shared_ptr<DescriptorPool> Create();
	};

}