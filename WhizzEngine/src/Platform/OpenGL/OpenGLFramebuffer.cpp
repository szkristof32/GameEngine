#include "pch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace WhizzEngine {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification)
		: m_Width(specification.Width), m_Height(specification.Height)
	{
		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		m_ColourAttachments.resize(specification.ColourAttachmentCount);
		for (uint32_t i = 0; i < specification.ColourAttachmentCount; i++)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_ColourAttachments[i]);
			glBindTexture(GL_TEXTURE_2D, m_ColourAttachments[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourAttachments[i], 0);
		}

		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthTexture);
			glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		for (uint32_t i = 0; i < m_ColourAttachments.size(); i++)
			glDeleteTextures(1, &m_ColourAttachments[i]);
		glDeleteTextures(1, &m_DepthTexture);
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Width, m_Height);
	}
	
	void OpenGLFramebuffer::Unbind(uint32_t width, uint32_t height) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
	}

	void OpenGLFramebuffer::BlitToScreen()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		glDrawBuffer(GL_BACK);
		glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, 1600, 900, GL_COLOR_BUFFER_BIT, GL_NEAREST); // TODO check real window size
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}