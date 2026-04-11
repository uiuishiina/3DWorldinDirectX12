#pragma once
#include<string>
#include <sstream>

//通常ログマクロ
#define LOG(...)	LogSystem::Log(__VA_ARGS__)

//特殊ログ
//改行なしログマクロ
#define	INLINE_LOG(...)	LogSystem::InlineLog(__VA_ARGS__)
//呼び出し元表示ログマクロ
#define CALLER_LOG(...)	LogSystem::Log(__VA_ARGS__,"[", __func__, "]")
//改行マクロ
#define LOG_LINE_BREAK(times)	LogSystem::LogLineBreak(times)
//変数変換マクロ
#define CHANGE_VALUE(X) #X, "=", (X)
//HRESULT変換ログマクロ
#define LOG_HR(expr) LogSystem::HRLog(expr)


class LogSystem final
{
	LogSystem() = default;
	~LogSystem() = default;

	static void GetArgs(const std::string& str)noexcept;

	template<typename T>
	static std::string ToLogString(const T& value) {
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}
	static std::string ToHRESULTString(long value);
public:
	template<typename... Args>
	static void Log(Args&&... args)noexcept {
		std::ostringstream oss;
		((oss << std::forward<Args>(args) << " "), ...);
		GetArgs(oss.str());
		LogLineBreak(1);
	}

	static void HRLog(long hr)noexcept {
		GetArgs(ToHRESULTString(hr));
		LogLineBreak(1);
	}

	template<typename... Args>
	static void InlineLog(Args&&... args)noexcept {
		std::ostringstream oss;
		((oss << ToLogString(std::forward<Args>(args)) << " "), ...);
		GetArgs(oss.str());
	}

	static void LogLineBreak(int times = 1)noexcept;
};