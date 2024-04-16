#include "window.hpp"
#include "../browser/browser.hpp"

HWND window::Create(bool isResizeable, bool hideTitleBar, bool show, int width, int height, int startX, int startY, const char* name, int icon, browser::browser* handle) {
    HICON hIcon = icon >= 0 ? LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(icon)) : LoadIcon(GetModuleHandle(nullptr), IDI_APPLICATION);

    WNDCLASSEX windowClassEx{};
    windowClassEx.cbSize = sizeof(WNDCLASSEX);
    windowClassEx.style = (isResizeable ? (CS_HREDRAW | CS_VREDRAW) : 0);
    windowClassEx.lpfnWndProc = HandleWindowMessage;
    windowClassEx.cbClsExtra = 0;
    windowClassEx.cbWndExtra = 0;
    windowClassEx.hInstance = GetModuleHandle(nullptr);
    windowClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClassEx.lpszMenuName = nullptr;
    windowClassEx.lpszClassName = "WindowClass";

    windowClassEx.hIcon = hIcon;
    windowClassEx.hIconSm = LoadIcon(windowClassEx.hInstance, IDI_APPLICATION);

    if (RegisterClassEx(&windowClassEx) == 0)
        return nullptr;

    DWORD windowFlags = WS_VISIBLE | WS_SYSMENU | WS_OVERLAPPED |
        (hideTitleBar ? (WS_POPUP) : (WS_CAPTION | WS_MINIMIZEBOX)) |
        (isResizeable ? ( WS_THICKFRAME | WS_MAXIMIZEBOX) : 0);

    const int windowX = startX > 0 ? startX : (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    const int windowY = startY > 0 ? startY : (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

    const HWND windowHandle = CreateWindow("WindowClass", name,
        windowFlags, windowX, windowY, width, height,
        NULL, NULL, GetModuleHandle(nullptr), NULL);

    if (show)
        ShowWindow(windowHandle, SW_SHOW);

    SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)handle);
    UpdateWindow(windowHandle);
    return windowHandle;
}

LRESULT window::HandleWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    const auto handle = reinterpret_cast<browser::browser*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            return 1L;
        case WM_SIZE:
            RECT bounds;
            GetClientRect(hWnd, &bounds);
            if (handle)
                handle->m_webViewController->put_Bounds(bounds);
            return 1L;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void window::Destroy(HWND handle) {
    if (handle)
        DestroyWindow(handle);
}

bool window::Init(browser::browser* handle) {
    handle->RegisterFunction("HandleWindowEvent", &window::HandleWindowEvent);
    return true;
}

bool window::HandleWindowEvent(browser::browser* handle, browser::js_args_t args) {
    if (args.Size() < 1)
        return false;

    const auto event = args.Get<0, std::string>();

    if (event == "minimize")
        return ShowWindow(handle->m_windowHandle, SW_MINIMIZE);

    if (event == "resize") {
        WINDOWPLACEMENT wp;
        wp.length = sizeof(WINDOWPLACEMENT);
        if (GetWindowPlacement(handle->m_windowHandle, &wp))
            return ShowWindow(handle->m_windowHandle, wp.showCmd != SW_SHOWMAXIMIZED ? SW_SHOWMAXIMIZED : SW_RESTORE);

        return false;
    }

    if (event == "close") {
        PostQuitMessage(0);
        return true;
    }

    return false;
}