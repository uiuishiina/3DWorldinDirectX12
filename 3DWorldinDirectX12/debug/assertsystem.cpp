#include"assertsystem.h"
#include"logsystem.h"
#include<cstdlib>

void AssertSystem::AssertBreak(bool value, const char* expr, const char* func, const char* file, int line) {
	if (!value) {
		LOG("[ ASSERT FAILED ]", expr);
		LOG("[ Func ]", func, "[ File ]", file, " : ", line);
		std::abort();
	}
}