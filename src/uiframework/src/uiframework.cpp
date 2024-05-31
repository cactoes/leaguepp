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

std::shared_ptr<ISystemWindow> CreateSystemWindow(const WINDOW_CONFIG& wc, const BROWSER_CONFIG& bc, const component::LAYOUT& layout, const component::ALIGN& align) {
    return std::make_shared<SystemWindow>(Window::Create(wc), bc, layout, align);
}

std::shared_ptr<ISystemWindow> CreateMessageBox(const std::string& title, const std::string& message, message_box_type type) {
    WINDOW_CONFIG config{};
    config.flags = { WINDOW_FLAG_SHOW, WINDOW_FLAG_HIDE_TITLE_BAR };
    config.size = { 200, 150 };
    config.name = title;
    config.icon = 1;

    BROWSER_CONFIG bwConfig{};

    auto systemWindow = CreateSystemWindow(config, bwConfig, component::LAYOUT::VERTICAL);
    systemWindow->EnableContextMenu(false);
    auto frameMain = systemWindow->GetWindowFrame();

    auto parent = frameMain->AddFrame("", false, component::LAYOUT::HORIZONTAL);
    auto iconFrame = parent->AddFrame("", false, component::LAYOUT::HORIZONTAL_AUTO, component::ALIGN::HORIZONTAL);

    switch (type) {
        case message_box_type::MB_ERROR:
            iconFrame->AddImage("media/error_icon.png", { 50, 50 });
            break;
        case message_box_type::MB_WARN:
            iconFrame->AddImage("media/warn_icon.png", { 50, 50 });
            break;
        case message_box_type::MB_INFO:
            iconFrame->AddImage("media/info_icon.png", { 50, 50 });
            break;
        default:
            break;
    }

    parent
        ->AddFrame("", false, component::LAYOUT::VERTICAL, component::ALIGN::CENTER)
        ->AddLabel(message);

    frameMain
        ->AddFrame("", false, component::LAYOUT::HORIZONTAL, component::ALIGN::CENTER)
        ->AddButton("ok", [&]() {
            systemWindow->CloseWindow();
        });

    return systemWindow;
}