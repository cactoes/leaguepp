#include "window.hpp"

#include "browser.hpp"

Window Window::Create(const WINDOW_CONFIG& config) {
    const HICON hIcon = config.iconId >= 0 ? LoadIconA(GetModuleHandleA(nullptr), MAKEINTRESOURCEA(config.iconId))
        : LoadIconA(GetModuleHandleA(nullptr), IDI_APPLICATION);

    const std::string wndclassName = config.name + " WindowClass";

    WNDCLASSEX windowClassEx{};
    windowClassEx.cbSize = sizeof(WNDCLASSEX);
    windowClassEx.style = (config.flags.Has<WINDOW_FLAG_RESIZEABLE>() ? (CS_HREDRAW | CS_VREDRAW) : 0);
    windowClassEx.lpfnWndProc = &Browser::OnWindowMessage;
    windowClassEx.cbClsExtra = 0;
    windowClassEx.cbWndExtra = 0;
    windowClassEx.hInstance = GetModuleHandleA(nullptr);
    windowClassEx.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    windowClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClassEx.lpszMenuName = nullptr;
    windowClassEx.lpszClassName = wndclassName.c_str();

    windowClassEx.hIcon = hIcon;
    windowClassEx.hIconSm = LoadIconA(windowClassEx.hInstance, IDI_APPLICATION);

    RegisterClassExA(&windowClassEx);

    const DWORD windowFlags = WS_VISIBLE | WS_SYSMENU | WS_OVERLAPPED |
        (config.flags.Has<WINDOW_FLAG_HIDE_TITLE_BAR>() ? (WS_POPUP) : (WS_CAPTION | WS_MINIMIZEBOX)) |
        (config.flags.Has<WINDOW_FLAG_RESIZEABLE>() ? ( WS_THICKFRAME | WS_MAXIMIZEBOX) : 0);

    const int windowX = config.pos.x > 0 ? config.pos.x : (GetSystemMetrics(SM_CXSCREEN) - config.size.x) / 2;
    const int windowY = config.pos.y > 0 ? config.pos.y : (GetSystemMetrics(SM_CYSCREEN) - config.size.y) / 2;

    const HWND windowHandle = CreateWindowA(wndclassName.c_str(),
        config.name.c_str(),
        windowFlags,
        windowX, windowY,
        config.size.x, config.size.y,
        NULL, NULL,
        GetModuleHandleA(nullptr),
        NULL);

    if (config.flags.Has<WINDOW_FLAG_SHOW>())
        ShowWindow(windowHandle, SW_SHOW);

    UpdateWindow(windowHandle);

    return Window(windowHandle, config.name, config.iconName);
}