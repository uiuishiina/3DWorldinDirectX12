#include"logsystem.h"
#include<Windows.h>

void LogSystem::GetArgs(const std::string& str)noexcept {
	OutputDebugStringA(str.c_str());
}

void LogSystem::LogLineBreak(int times)noexcept {
	for (int i = 0; i < times; ++i) {
		OutputDebugStringA("\n");
	}
}