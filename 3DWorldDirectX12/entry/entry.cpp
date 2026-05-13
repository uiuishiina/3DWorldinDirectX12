#define WIN32_LEAN_AND_MEAN
#include"../entry/Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	Application App;
	if (!App.initalize_App(hInstance)) {
		return -1;
	}
	App.run_App();

	return 0;
}