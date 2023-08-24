#pragma once

#include "WhizzEngine/Rendering/Context.h"

namespace WhizzEngine {

	class WGLContext : public GraphicsContext
	{
	public:
		WGLContext(const void* windowHandle);
		~WGLContext();

		virtual void Swap() override;
		virtual void WaitForIdle() override {};
	private:
		const void* m_WindowHandle;
		const void* m_DeviceContext;
		const void* m_RenderingContext;
	};

}