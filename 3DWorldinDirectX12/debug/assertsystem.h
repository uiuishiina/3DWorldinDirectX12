#pragma once

#define ASSERT(expr) AssertSystem::AssertBreak((expr),#expr,__func__,__FILE__, __LINE__)
#define HRASSERT(expr) AssertSystem::HRAssert((expr),#expr,__func__,__FILE__, __LINE__)

class AssertSystem final
{
	AssertSystem() = default;
	~AssertSystem() = default;
public:
	static void AssertBreak(bool value, const char* expr, const char* func, const char* file, int line);
	static void AssertBreak(void* value, const char* expr, const char* func, const char* file, int line);

	static void HRAssert(long value, const char* expr, const char* func, const char* file, int line);
};
