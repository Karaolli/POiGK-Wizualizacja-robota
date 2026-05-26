#include "Window.h"
#include "App.h"

#include <windowsx.h>

Window::Window(int width, int height, LPCWSTR title)
    : width(width), height(height) {
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hbrBackground = nullptr;
    wc.lpszClassName = className;
    RegisterClassExW(&wc);

    RECT wr = { 0, 0, width, height };
    AdjustWindowRectEx(&wr, WS_OVERLAPPEDWINDOW, FALSE, 0);

    HWND hwnd = CreateWindowExW(
        0, className, title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, GetModuleHandle(nullptr), this
    );
    ShowWindow(hwnd, SW_SHOW);
}

Window::~Window() {
    DestroyWindow(hwnd);
    UnregisterClass(className, GetModuleHandle(nullptr));
}

bool Window::ProcessMessages()
{
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT)
            return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Window* self = nullptr;

    if (msg == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        self = reinterpret_cast<Window*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        self->hwnd = hwnd;
    }
    else {
        self = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (self)
        return self->HandleMessage(msg, wParam, lParam);

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {

    case WM_ENTERSIZEMOVE:
        SetTimer(hwnd, 1, 1, nullptr);
		return 0;
	case WM_EXITSIZEMOVE:
        KillTimer(hwnd, 1);
        app->Update();
        app->Render(1u);
        return 0;

    case WM_TIMER:
        if (wParam == 1) {
            app->Update();
            app->Render(1u);
        }
        return 0;

    case WM_SIZE:
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED) {
            if (app) {
                app->gfx.OnResize(width, height);
                app->Update();
                app->Render(1u);
            }
        }
        return 0;

    case WM_KEYDOWN:
        app->input.keys[wParam] = true;
        return 0;
    case WM_KEYUP:
        app->input.keys[wParam] = false;
        return 0;

    case WM_LBUTTONDOWN:
        app->input.mouseButtons[0] = true;
        return 0;
    case WM_LBUTTONUP:
        app->input.mouseButtons[0] = false;
        return 0;
    case WM_RBUTTONDOWN:
        app->input.mouseButtons[1] = true;
        SetCapture(hwnd);
        return 0;
    case WM_RBUTTONUP:
        app->input.mouseButtons[1] = false;
        ReleaseCapture();
        return 0;
    case WM_MBUTTONDOWN:
        app->input.mouseButtons[2] = true;
        return 0;
    case WM_MBUTTONUP:
        app->input.mouseButtons[2] = false;
        return 0;

    case WM_MOUSEMOVE: {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
        app->input.mouseDX += x - app->input.lastMouseX;
        app->input.mouseDY += y - app->input.lastMouseY;
        app->input.lastMouseX = x;
        app->input.lastMouseY = y;
        return 0;
    }
    case WM_MOUSEWHEEL:
        app->input.mouseScrollDelta += GET_WHEEL_DELTA_WPARAM(wParam) / 120.0f;
        return 0;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_KILLFOCUS:

        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}