#pragma once
struct HWND__;
using HWND = HWND__*;

class DrawSystem final
{
public:
	[[nodiscard]] bool Initialize(HWND HWND,int width,int height);
};