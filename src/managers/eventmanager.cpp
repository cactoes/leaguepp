#include "eventmanager.hpp"

void event_manager::Setup(browser::browser* handle) {
    handle->RegisterFunction("HandleWindowEvent", CREATE_REGISTRATION_MEMBER(event_manager::HandleWindowEvent));
}

bool event_manager::HandleWindowEvent(browser::browser* handle, browser::js_args_t args) {
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