#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// This ignores all warnings raised inside external headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace WhizzEngine {

	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::qua<T, Q>& quaternion)
{
	return os << glm::to_string(quaternion);
}

// Core log macros
#define WZ_CORE_TRACE(...)		::WhizzEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define WZ_CORE_INFO(...)		::WhizzEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define WZ_CORE_WARN(...)		::WhizzEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define WZ_CORE_ERROR(...)		::WhizzEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define WZ_CORE_CRITICAL(...)	::WhizzEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define WZ_TRACE(...)			::WhizzEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define WZ_INFO(...)			::WhizzEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define WZ_WARN(...)			::WhizzEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define WZ_ERROR(...)			::WhizzEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define WZ_CRITICAL(...)		::WhizzEngine::Log::GetClientLogger()->critical(__VA_ARGS__)