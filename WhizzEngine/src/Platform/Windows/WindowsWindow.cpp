#include "pch.h"
#include "WindowsWindow.h"

#include "WhizzEngine/Events/EventBus.h"
#include "WhizzEngine/Events/ApplicationEvent.h"
#include "WhizzEngine/Events/KeyEvent.h"
#include "WhizzEngine/Events/MouseEvent.h"

#include <windowsx.h>

namespace WhizzEngine {

	static LRESULT CALLBACK WindowProcess(HWND, UINT, WPARAM, LPARAM);

	WindowsWindow::WindowsWindow(uint32_t width, uint32_t height, const std::string& title)
		: m_Width(width), m_Height(height)
	{
		const wchar_t CLASS_NAME[] = L"WindowsWindow";

		WNDCLASS wc{};
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.lpfnWndProc = WindowProcess;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = CLASS_NAME;

		RegisterClass(&wc);

		int displayWidth = GetSystemMetrics(SM_CXSCREEN);
		int displayHeight = GetSystemMetrics(SM_CYSCREEN);
		std::wstring w_title = std::wstring(title.begin(), title.end());

		m_Window = CreateWindowEx(0, CLASS_NAME, w_title.c_str(), WS_OVERLAPPEDWINDOW, (displayWidth - width) / 2, (displayHeight - height) / 2, width, height, nullptr, nullptr, GetModuleHandle(NULL), nullptr);
		if (!m_Window)
			exit(-1);

		ShowWindow(m_Window, SW_SHOW);
	}

	WindowsWindow::~WindowsWindow()
	{
	}

	void WindowsWindow::Update()
	{
		MSG msg{};
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	static LRESULT CALLBACK WindowProcess(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		switch (message)
		{
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				break;
			}
			case WM_CLOSE:
			{
				auto event = std::make_shared<WindowCloseEvent>();
				EventBus::PostEvent(event);
				DestroyWindow(window);
				break;
			}
			case WM_SIZE:
			{
				auto event = std::make_shared<WindowResizeEvent>(LOWORD(lparam), HIWORD(lparam));
				EventBus::PostEvent(event);
				break;
			}
			case WM_KEYDOWN:
			{
				auto event = std::make_shared<KeyPressedEvent>((KeyCode)wparam, lparam & 0b1);
				EventBus::PostEvent(event);
				break;
			}
			case WM_KEYUP:
			{
				auto event = std::make_shared<KeyReleasedEvent>((KeyCode)wparam);
				EventBus::PostEvent(event);
				break;
			}
			case WM_CHAR:
			{
				auto event = std::make_shared<KeyTypedEvent>((KeyCode)wparam);
				EventBus::PostEvent(event);
				break;
			}
			case WM_LBUTTONDOWN:
			{
				auto event = std::make_shared<MouseButtonPressedEvent>(MouseCodes::ButtonLeft);
				EventBus::PostEvent(event);
				break;
			}
			case WM_LBUTTONUP:
			{
				auto event = std::make_shared<MouseButtonReleasedEvent>(MouseCodes::ButtonLeft);
				EventBus::PostEvent(event);
				break;
			}
			case WM_RBUTTONDOWN:
			{
				auto event = std::make_shared<MouseButtonPressedEvent>(MouseCodes::ButtonRight);
				EventBus::PostEvent(event);
				break;
			}
			case WM_RBUTTONUP:
			{
				auto event = std::make_shared<MouseButtonReleasedEvent>(MouseCodes::ButtonRight);
				EventBus::PostEvent(event);
				break;
			}
			case WM_MBUTTONDOWN:
			{
				auto event = std::make_shared<MouseButtonPressedEvent>(MouseCodes::ButtonMiddle);
				EventBus::PostEvent(event);
				break;
			}
			case WM_MBUTTONUP:
			{
				auto event = std::make_shared<MouseButtonReleasedEvent>(MouseCodes::ButtonMiddle);
				EventBus::PostEvent(event);
				break;
			}
			case WM_MOUSEMOVE:
			{
				int x = GET_X_LPARAM(lparam);
				int y = GET_Y_LPARAM(lparam);
				RECT size{};
				GetWindowRect(window, &size);
				auto event = std::make_shared<MouseMovedEvent>((float)x / (float)(size.right - size.left), (float)y / (float)(size.bottom - size.top));
				EventBus::PostEvent(event);
				break;
			}
			case WM_MOUSEWHEEL:
			{
				float xOffset = 0.0f;
				float yOffset = (float)GET_WHEEL_DELTA_WPARAM(wparam) / (float)WHEEL_DELTA;
				auto event = std::make_shared<MouseScrolledEvent>(xOffset, yOffset);
				EventBus::PostEvent(event);
				break;
			}
		}

		return DefWindowProc(window, message, wparam, lparam);
	}

}