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


std::string LogSystem::ToHRESULTString(long value) {
    char* msgBuf = nullptr;

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        value,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&msgBuf,
        0,
        NULL
    );

    std::ostringstream oss;
    oss << "hr = " << "0x" << std::hex << value << " : "
        << (msgBuf ? msgBuf : "Unknown error");

    if (msgBuf) LocalFree(msgBuf);

    return oss.str();
}