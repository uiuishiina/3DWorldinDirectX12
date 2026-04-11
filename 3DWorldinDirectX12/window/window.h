#pragma once
#include<utility>

struct HINSTANCE__;
using HINSTANCE = HINSTANCE__*;

struct HWND__;
using HWND = HWND__*;

class window final
{
	window() = default;
	~window() = default;

	HWND MyHandle_{};
	std::pair<int, int> WindowSize_{};
public:
	static window& Instance()noexcept {
		static window instance_;
		return instance_;
	}

	[[nodiscard]] bool CreateMainWindow(HINSTANCE hInstance, int width, int height);

	[[nodiscard]] HWND GetHandle()const noexcept;

	[[nodiscard]] std::pair<int, int> GetSize()const noexcept;

	[[nodiscard]] bool MessageLoop()noexcept;
};