#pragma once

#include "WhizzEngine/Core/Core.h"
#include "WhizzEngine/Core/Log.h"
#include <filesystem>

#ifdef WZ_ENABLE_ASSERTS
	// Alternatively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of
	// having the format inside the default message
	#define WZ_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { WZ##type##ERROR(msg, __VA_ARGS__); WZ_DEBUGBREAK(); } }
	#define WZ_INTERNAL_ASSERT_WITH_MSG(type, check, ...) WZ_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define WZ_INTERNAL_ASSERT_NO_MSG(type, check) WZ_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", WZ_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define WZ_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define WZ_INTERNAL_ASSERT_GET_MACRO(...) WZ_EXPAND_MACRO(WZ_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, WZ_INTERNAL_ASSERT_WITH_MSG, WZ_INTERNAL_ASSERT_NO_MSG))

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define WZ_ASSERT(...) WZ_EXPAND_MACRO(WZ_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
	#define WZ_CORE_ASSERT(...) WZ_EXPAND_MACRO(WZ_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
	#define WZ_ASSERT(...)
	#define WZ_CORE_ASSERT(...)
#endif