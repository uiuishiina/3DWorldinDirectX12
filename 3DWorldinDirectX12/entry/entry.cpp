#include<Windows.h>
#include"../window/window.h"
#include"../drawsystem/drawsystem.h"
/*
auto hr = HRESULT{ E_INVALIDARG };
	DEBUG_HR_ASSERT(hr);
*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	if (window::Instance().CreateMainWindow(hInstance, 1280, 720)) {
		DrawSystem d;
		auto [w, h] = window::Instance().GetSize();
		d.Initialize(window::Instance().GetHandle(), w, h);
		while (window::Instance().MessageLoop()) {

		}
	}
	return 0;
}