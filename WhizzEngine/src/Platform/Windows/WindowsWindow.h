#pragma once

#include "WhizzEngine/Core/Window.h"

namespace WhizzEngine {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(uint32_t width, uint32_t height, const std::string& title);
		~WindowsWindow();

		virtual void Update() override;

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual const void* GetNativeHandle() const override { return m_Window; }
		virtual void WaitForEvents() const override;
	private:
		const void* m_Window;
		uint32_t m_Width;
		uint32_t m_Height;
	};

}