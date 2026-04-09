#include<Windows.h>
#include"../debug/assertsystem.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int i = 100;
	ASSERT(0==i);
}