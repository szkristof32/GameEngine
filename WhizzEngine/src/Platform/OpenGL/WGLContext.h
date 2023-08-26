#pragma once

#include "WhizzEngine/Rendering/Context.h"

#include <Windows.h>

namespace WhizzEngine {

	class WGLContext : public GraphicsContext
	{
	public:
		WGLContext(HWND windowHandle);
		~WGLContext();

		virtual void Swap() override;
	private:
		HWND m_WindowHandle;
		HDC m_DeviceContext;
		HGLRC m_RenderingContext;
	};

}