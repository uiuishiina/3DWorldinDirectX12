#pragma once

#define ASSERT(expr) AssertSystem::AssertBreak((expr),#expr,__FUNCSIG__,__FILE__, __LINE__)

class AssertSystem final
{
	AssertSystem() = default;
	~AssertSystem() = default;
public:
	static void AssertBreak(bool value, const char* expr, const char* func, const char* file, int line);
};