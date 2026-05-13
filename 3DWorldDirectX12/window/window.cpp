#include"window.h"

namespace {
    LRESULT CALLBACK static_window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (msg == WM_NCCREATE) {
            auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            auto pWnd = reinterpret_cast<window*>(cs->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            return pWnd->window_proc(hwnd, msg, wParam, lParam);
        }
        auto pWnd = reinterpret_cast<window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (pWnd) {
            return pWnd->window_proc(hwnd, msg, wParam, lParam);
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

[[nodiscard]] bool window::create_window(HINSTANCE hInstance, UINT16 width, UINT16 height) {

    WNDCLASS wc{};
    wc.lpfnWndProc = static_window_proc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SampleWindowClass";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (!RegisterClass(&wc)) {
        return false;
    }

    hwnd_ = CreateWindowEx(
        0,                        //    拡張スタイル
        wc.lpszClassName,        //    クラス名
        L"Sample Window",        //    ウィンドウタイトル
        WS_OVERLAPPEDWINDOW,    //    ウィンドウスタイル
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,    //    位置とサイズ
        nullptr,                //    親ウィンドウ
        nullptr,                //    メニューハンドル
        hInstance,                //    インスタンスハンドル
        this                    //    追加パラメータ
    );

    if (!hwnd_) {
        return false;
    }

    ShowWindow(hwnd_, SW_SHOW);
    return true;
}

LRESULT	window::window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_NCDESTROY: {
        auto self = reinterpret_cast<window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (self) {
            SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
        }
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}