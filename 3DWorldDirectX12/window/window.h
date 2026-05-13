#pragma once
#include<Windows.h>

class window final
{
	HWND hwnd_{};
	struct window_size {
		UINT16 width;
		UINT16 height;
	};
	window_size size_{};
public:
	[[nodiscard]] bool create_window(HINSTANCE hInstance, UINT16 width, UINT16 height);

	LRESULT	window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	[[nodiscard]] HWND get_hwnd()const noexcept { return hwnd_; }

	[[nodiscard]] window_size get_size()const noexcept { return size_; }
};