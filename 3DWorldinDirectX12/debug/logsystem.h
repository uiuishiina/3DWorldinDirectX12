#pragma once
#include<string>
#include <sstream>

//通常ログマクロ
#define LOG(...)	LogSystem::Log(__VA_ARGS__)

//特殊ログ
//改行なしログマクロ
#define	INLINE_LOG(...)	LogSystem::InlineLog(__VA_ARGS__)
//呼び出し元表示ログマクロ
#define CALLER_LOG(...)	LogSystem::Log("[", __FUNCSIG__, "]","LOG:",__VA_ARGS__)
//改行マクロ
#define LOG_LINE_BREAK(times)	LogSystem::LogLineBreak(times)


class LogSystem final
{
	LogSystem() = default;
	~LogSystem() = default;

	static void GetArgs(const std::string& str)noexcept;
public:
	template<typename... Args>
	static void Log(Args&&... args)noexcept {
		std::ostringstream oss;
		((oss << std::forward<Args>(args) << " "), ...);
		GetArgs(oss.str() + "\n");
	}

	template<typename... Args>
	static void InlineLog(Args&&... args)noexcept {
		std::ostringstream oss;
		((oss << std::forward<Args>(args) << " "), ...);
		GetArgs(oss.str());
	}

	static void LogLineBreak(int times = 1)noexcept;
};