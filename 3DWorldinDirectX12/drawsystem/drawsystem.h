#pragma once
struct HWND__;
using HWND = HWND__*;
#include<memory>

class DrawSystem final
{
public:
	DrawSystem();
	~DrawSystem();

	DrawSystem(const DrawSystem&) = delete;
	DrawSystem& operator=(const DrawSystem&) = delete;
	DrawSystem(DrawSystem&&) = delete;
	DrawSystem& operator=(DrawSystem&&) = delete;

	[[nodiscard]] bool Initialize(HWND HWND, int width, int height);

private:
	class Impl;
	std::unique_ptr<Impl> impl_;
};