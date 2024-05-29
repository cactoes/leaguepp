#include "uiframework.hpp"
#include "systemwindow.hpp"

void SystemPollWindowEvents(void) {
    MSG msg{};
    // while (msg.message != WM_QUIT) { // PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)
        while (GetMessageA(&msg, nullptr, 0, 0) != 0) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    // }
}

std::unique_ptr<ISystemWindow> CreateSystemWindow(const WINDOW_CONFIG& wc, const BROWSER_CONFIG& bc, const component::LAYOUT& layout, const component::ALIGN& align) {
    return std::make_unique<SystemWindow>(Window::Create(wc), bc, layout, align);
}