#include "systemwindow.hpp"

void SystemWindow::OnSetContextMenuData(Browser*, JSArgs) {
    for (const auto& [group, items] : m_contextMenu) {
        std::vector<std::any> args = { group };

        for (const auto& [name, _] : items)
            args.push_back(name);

        m_browser.CallJSFunction("createContextMenuGroup", args);
    }
}

void SystemWindow::OnContextMenuItemClicked(Browser*, JSArgs args) {
    const auto group = args.at(0).As<std::string>();
    const auto item = args.at(1).As<std::string>();
    m_contextMenu.at(group).at(item).Run();
}

std::shared_ptr<component::IFrame> SystemWindow::GetWindowFrame(void) {
    return std::static_pointer_cast<component::IFrame>(m_windowFrame);
}

void SystemWindow::CloseWindow(void) {
    DestroyWindow(m_browser.GetWindow().GetWindowHandle());
    SystemWindow::s_windows.erase(m_id);
}

void SystemWindow::AddContextMenuItem(const std::string& group, const std::string& name, const FnOnContextMenuItemClicked& callback) {
    m_contextMenu.at(group)[name] = callback;
}

void SystemWindow::CreateContextMenuGroup(const std::string& name) {
    m_contextMenu[name] = {};
}

void SystemWindow::EnableContextMenu(bool state) {
    m_browser.AddDOMContentLoadedHandler([this, state](Browser*) {
        m_browser.CallJSFunction("setContextMenuEnabled", { state });
    });
}