#include "systemwindow.hpp"

std::shared_ptr<component::IFrame> SystemWindow::GetWindowFrame(void) {
    return std::static_pointer_cast<component::IFrame>(m_windowFrame);
}

void SystemWindow::CloseWindow(void) {
    DestroyWindow(m_browser.GetWindow().GetWindowHandle());
}