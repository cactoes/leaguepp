#include "browser.hpp"

#include "../window/window.hpp"
#include "typehelpers.hpp"

void browser::browser::Start() {
    MSG msg{};
    event_bus::message_t eventMessage = {};

    while (WM_QUIT != msg.message) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        while (m_bus.getMessage(&eventMessage)) {
            std::string script = "__core_call_js__(\"" + eventMessage.name + "\",";

            for (const auto& arg : eventMessage.args)
                script += AnyToMappedString(arg) + ",";

            script[script.size() - 1] = ')';

            m_webView2->ExecuteScript(StringToWideString(script).c_str(), nullptr);
        }
    }

    window::Destroy(m_windowHandle);
}

void browser::browser::CallJSFunction(const std::string& name, const std::vector<std::any>& args) {
    m_bus.AddMessage({ .name = name, .args = args });
}

void browser::browser::RegisterFunction(const std::string& name, std::function<std::any(browser*, js_args_t)> func) {
    m_methodMap[name] = func;
}

void browser::browser::AddDOMContentLoadedHandler(std::function<void(browser*)> func) {
    m_eventMethodMap["DOMContentLoaded"].emplace_back(func);
}

void browser::browser::MoveWindow(browser*, js_args_t jsArgs) {
    RECT rect;
    GetWindowRect(m_windowHandle, &rect);

    int newLeft = rect.left + jsArgs.Get<0, int>();
    int newTop = rect.top + jsArgs.Get<1, int>();
    int newWidth = rect.right - rect.left;
    int newHeight = rect.bottom - rect.top;

    SetWindowPos(m_windowHandle, nullptr, newLeft, newTop, newWidth, newHeight,
        SWP_NOZORDER | SWP_NOACTIVATE);
}

void browser::browser::OnDomContentLoaded() {
    if (m_eventMethodMap.contains("DOMContentLoaded"))
        for (const auto& pFunction : m_eventMethodMap.at("DOMContentLoaded"))
            pFunction(this);
}