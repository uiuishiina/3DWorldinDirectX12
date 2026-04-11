#include"assertsystem.h"
#include"logsystem.h"
#include<cstdlib>
#include<Windows.h>

static bool IsError(HRESULT value) {
	return FAILED(value);
}

template<typename T>
static bool IsError(T&& value) {
	return !value;
}

void AssertSystem::AssertBreak(bool value, const char* expr, const char* func, const char* file, int line) {
	if (IsError(value)) {

		LOG_LINE_BREAK(1);
		LOG("===================================================================================================");
		LOG("[ ASSERT FAILED POINT ]", expr);
		LOG("[ Func ]", func, "[ File ]", file, " : ", line);
		LOG("===================================================================================================");
		LOG_LINE_BREAK(1);
		std::abort();
	}
}

void AssertSystem::HRAssertBreak(long value, const char* expr, const char* func, const char* file, int line) {
	if (IsError(static_cast<HRESULT>(value))) {
		LOG_LINE_BREAK(1);
		LOG("===================================================================================================");
		LOG("[ ASSERT FAILED POINT ]", expr);
		LOG_HR(value);
		LOG("[ Func ]", func, "[ File ]", file, " : ", line);
		LOG("===================================================================================================");
		LOG_LINE_BREAK(1);
		std::abort();
	}
}