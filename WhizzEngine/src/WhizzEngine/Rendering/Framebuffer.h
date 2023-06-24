#pragma once

#include "WhizzEngine/Core/Core.h"

#include <memory>

namespace WhizzEngine {

	struct FramebufferSpecification
	{
		uint32_t Width;
		uint32_t Height;

		uint32_t ColourAttachmentCount;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind(uint32_t width = 1600, uint32_t height = 900) const = 0;

		virtual void BlitToScreen() = 0;

		WZ_CAST_TO(Framebuffer);

		static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& specification);
	};

}