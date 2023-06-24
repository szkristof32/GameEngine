#pragma once

#include "WhizzEngine/Rendering/Framebuffer.h"

#include <vector>

namespace WhizzEngine {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		~OpenGLFramebuffer();

		virtual void Bind() const override;
		virtual void Unbind(uint32_t width = 1600, uint32_t height = 900) const override;

		virtual void BlitToScreen() override;
	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;

		std::vector<uint32_t> m_ColourAttachments;
		uint32_t m_DepthTexture;
	};

}