#include "pch.h"
#include "Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace WhizzEngine {

	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specification)
	{
		return std::make_shared<OpenGLFramebuffer>(specification);
	}

}