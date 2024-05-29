#pragma once

#ifndef __SYSTEMWINDOW_HPP__
#define __SYSTEMWINDOW_HPP__

#include "components/frame.hpp"
#include "window.hpp"
#include "browser.hpp"

class SystemWindow : public ISystemWindow {
public:
    SystemWindow(const Window& window, const BROWSER_CONFIG& config, const component::LAYOUT& layout, const component::ALIGN& align)
        : m_browser(Browser(window, config)) {
        m_windowFrame = std::make_shared<component::Frame>("", &m_browser, "main", false, layout, align);
        m_browser.RegisterFunction("RenderLayout", &SystemWindow::OnRenderLayout, this);
    }

    void OnRenderLayout(Browser*, JSArgs) {
        GetWindowFrame()->Render();
    }

    std::shared_ptr<component::IFrame> GetWindowFrame(void) override;

private:
    Browser m_browser;
    std::shared_ptr<component::IFrame> m_windowFrame;
};

#endif // __SYSTEMWINDOW_HPP__