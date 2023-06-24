#pragma once

#include "WhizzEngine/Core/PlatformDetection.h"

#ifdef WZ_DEBUG
	#if defined(WZ_PLATFORM_WINDOWS)
		#define WZ_DEBUGBREAK() __debugbreak()
	#elif defined(WZ_PLATFORM_LINUX)
		#include <signal.h>
		#define WZ_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define WZ_ENABLE_ASSERTS
#else
	#define WZ_DEBUGBREAK()
#endif

#define WZ_EXPAND_MACRO(x) x
#define WZ_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define WZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define WZ_CAST_TO(extend)	template<typename T, typename std::enable_if<std::is_base_of<extend, T>::value>::type* = nullptr> \
							T& As() { return dynamic_cast<T&>(*this); }