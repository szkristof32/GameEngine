#include "pch.h"
#include "WGLContext.h"

#include <glad/glad.h>

namespace WhizzEngine {

	static void OpenGLErrorCallback(GLenum source, GLenum type, GLenum id, GLenum severity, GLsizei length, const char* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_NOTIFICATION:	WZ_CORE_TRACE("OpenGL notification: {0}", message); return;
			case GL_DEBUG_SEVERITY_LOW:				WZ_CORE_INFO("OpenGL info: {0}", message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:			WZ_CORE_WARN("OpenGL warning: {0}", message); return;
			case GL_DEBUG_SEVERITY_HIGH:			WZ_CORE_ERROR("OpenGL error: {0}", message); WZ_CORE_ASSERT(false); return;
		}
	}


	WGLContext::WGLContext(HWND windowHandle)
		: m_WindowHandle(windowHandle)
	{
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_BITMAP |
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER |
			PFD_TYPE_RGBA,
			24,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			32,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};
		m_DeviceContext = GetDC(m_WindowHandle);
		int pixelFormat = ChoosePixelFormat(m_DeviceContext, &pfd);
		SetPixelFormat(m_DeviceContext, pixelFormat, &pfd);

		m_RenderingContext = wglCreateContext(m_DeviceContext);
		wglMakeCurrent(m_DeviceContext, m_RenderingContext);
		gladLoadGL();

#ifdef WZ_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLErrorCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		WZ_CORE_INFO("OpenGL Info:");
		WZ_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		WZ_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		WZ_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	WGLContext::~WGLContext()
	{
		ReleaseDC(m_WindowHandle, m_DeviceContext);
		wglDeleteContext(m_RenderingContext);
	}

	void WGLContext::Swap()
	{
		SwapBuffers(m_DeviceContext);
	}

}