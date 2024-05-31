#pragma once

#ifndef __SYSTEMWINDOW_HPP__
#define __SYSTEMWINDOW_HPP__

#include "components/frame.hpp"
#include "window.hpp"
#include "browser.hpp"

#include <map>

class SystemWindow : public ISystemWindow {
public:
    SystemWindow(const Window& window, const BROWSER_CONFIG& config, const component::LAYOUT& layout, const component::ALIGN& align)
        : m_browser(Browser(window, config)) {
        m_windowFrame = std::make_shared<component::Frame>("", &m_browser, "main", false, layout, align);
        m_browser.RegisterFunction("RenderLayout", &SystemWindow::OnRenderLayout, this);
        m_browser.RegisterFunction("SetContextMenuData", &SystemWindow::OnSetContextMenuData, this);
        m_browser.RegisterFunction("OnContextMenuItemClicked", &SystemWindow::OnContextMenuItemClicked, this);
    }

    void OnRenderLayout(Browser*, JSArgs) {
        GetWindowFrame()->Render();
    }

    void OnSetContextMenuData(Browser*, JSArgs);
    void OnContextMenuItemClicked(Browser*, JSArgs args);

    std::shared_ptr<component::IFrame> GetWindowFrame(void) override;
    void CloseWindow(void) override;
    void AddContextMenuItem(const std::string& group, const std::string& name, const FnOnContextMenuItemClicked& callback) override;
    void CreateContextMenuGroup(const std::string& name) override;
    void EnableContextMenu(bool state) override;

private:
    Browser m_browser;
    std::shared_ptr<component::IFrame> m_windowFrame;
    std::map<std::string, std::map<std::string,FnOnContextMenuItemClicked>> m_contextMenu;
};

#endif // __SYSTEMWINDOW_HPP__