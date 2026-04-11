#include<Windows.h>
#include"window.h"
#include"../debug/debugsystem.h"

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
[[nodiscard]] bool window::CreateMainWindow(HINSTANCE hInstance, int width, int height) {

    const wchar_t CLASS_NAME[] = L"MyDX12WindowClass";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClassW(&wc);

    MyHandle_ = CreateWindowExW(
        0,
        CLASS_NAME,
        L"DX12 Viewer",
        WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL,
        NULL,
        hInstance,
        this
    );

    DEBUG_ASSERT(MyHandle_);

    ShowWindow(MyHandle_, SW_SHOW);
    
    WindowSize_ = { width,height };

    return true;
}

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto wnd = reinterpret_cast<window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hwnd);
        }
        return 0;
    case WM_MOUSEWHEEL:
    {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        return 0;
    }
    case WM_NCCREATE:
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        window* wnd = static_cast<window*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)wnd);
        return TRUE;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

[[nodiscard]] bool window::MessageLoop()noexcept {
    DEBUG_ASSERT(MyHandle_);

    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        // メッセージ処理
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

[[nodiscard]] HWND window::GetHandle()const noexcept {
    DEBUG_ASSERT(MyHandle_);
    return MyHandle_;
}

[[nodiscard]] std::pair<int, int> window::GetSize()const noexcept {
    DEBUG_ASSERT(MyHandle_);
    return WindowSize_;
}