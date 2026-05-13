#pragma once
#include"../window/window.h"

class Application final
{
	window window_{};

	[[nodiscard]] bool message_loop();
public:
	[[nodiscard]] bool initalize_App(HINSTANCE hInstance);

	void run_App();
};