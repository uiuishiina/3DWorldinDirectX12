#include"Application.h"

//----------------------------------------------------------------------------------------------------

[[nodiscard]] bool Application::initialize_App(HINSTANCE hInstance) {

	if (!window_.create_window(hInstance, 1280, 720)) {
		return false;
	}

    if (!renderer_.initialize_renderer(window_.get_hwnd())) {
        return false;
    }

	return true;
}

//----------------------------------------------------------------------------------------------------

void Application::run_App() {
	while (message_loop()) {

	}
}

//----------------------------------------------------------------------------------------------------

[[nodiscard]] bool Application::message_loop() {
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}