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
    auto ptr = std::make_shared<SystemWindow>(Window::Create(wc), bc, layout, align);
    SystemWindow::s_windows[ptr->m_id] = ptr;
    return ptr;
}

std::shared_ptr<ISystemWindow> CreateMessageBox(const std::string& title, int iconId, const std::string& iconName, const std::string& message, message_box_type type) {
    WINDOW_CONFIG config{};
    config.flags = { WINDOW_FLAG_SHOW, WINDOW_FLAG_HIDE_TITLE_BAR };

    int extraWidth = 0;
    if (message.size() > 30)
        // 1 is the scaling factor
        extraWidth = (int)((double)message.size() * 1);

    config.size = { 200 + extraWidth, 150 };
    config.name = title;
    config.iconId = iconId;
    config.iconName = iconName;

    BROWSER_CONFIG bwConfig{};

    auto systemWindow = CreateSystemWindow(config, bwConfig, component::LAYOUT::VERTICAL);
    systemWindow->EnableContextMenu(false);
    auto frameMain = systemWindow->GetWindowFrame();

    auto parent = frameMain->AddFrame("", false, component::LAYOUT::HORIZONTAL);
    auto iconFrame = parent->AddFrame("", false, component::LAYOUT::VERTICAL_AUTO, component::ALIGN::HORIZONTAL);

    switch (type) {
        case message_box_type::MB_ERROR:
            iconFrame->AddImage("error_icon.png", { 50, 50 });
            break;
        case message_box_type::MB_WARN:
            iconFrame->AddImage("warn_icon.png", { 50, 50 });
            break;
        case message_box_type::MB_INFO:
            iconFrame->AddImage("info_icon.png", { 50, 50 });
            break;
        default:
            break;
    }

    auto msgFrame = parent->AddFrame("", false, component::LAYOUT::HORIZONTAL, component::ALIGN::CENTER);
    msgFrame->AddFrame("", false, component::LAYOUT::VERTICAL_AUTO);
    msgFrame->AddFrame("", false, component::LAYOUT::VERTICAL_AUTO);
    msgFrame->AddLabel(message);

    frameMain
        ->AddFrame("", false, component::LAYOUT::HORIZONTAL, component::ALIGN::CENTER)
        ->AddButton("ok", [systemWindow]() {
            systemWindow->CloseWindow();
        });

    return systemWindow;
}