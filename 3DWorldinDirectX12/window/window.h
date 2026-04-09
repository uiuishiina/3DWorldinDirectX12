#pragma once
#include<utility>

struct HINSTANCE_;
using HINSTANCE = HINSTANCE_*;

struct HWND_;
using HWND = HWND_*;

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

	[[nodiscard]] bool CreateWindow(HINSTANCE hInstance);
};