#pragma once

#include <memory>
#include <string>

namespace WhizzEngine {

	class Window
	{
	public:
		~Window() = default;

		virtual void Update() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const void* GetNativeHandle() const = 0;

		static std::shared_ptr<Window> Create(uint32_t width, uint32_t height, const std::string& title);
	};

}