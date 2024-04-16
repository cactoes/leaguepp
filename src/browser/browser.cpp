#include "browser.hpp"

#include "../window/window.hpp"
#include "typehelpers.hpp"

void browser::Browser::Start() {
    window::Init(this);

    MSG msg{};
    EventBus::message_t eventMessage = {};

    while (WM_QUIT != msg.message) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        while (m_isReady && m_bus.GetMessage(&eventMessage)) {
            std::string script = "__core_call_js__(\"" + eventMessage.name + "\",";

            for (const auto& arg : eventMessage.args)
                script += AnyToMappedString(arg) + ",";

            script[script.size() - 1] = ')';

            m_webView2->ExecuteScript(StringToWideString(script).c_str(), nullptr);
        }
    }

    window::Destroy(m_windowHandle);
}

void browser::Browser::CallJSFunction(const std::string& name, const std::vector<std::any>& args) {
    m_bus.AddMessage({ .name = name, .args = args });
}

void browser::Browser::RegisterFunction(const std::string& name, std::function<std::any(Browser*, js_args_t)> func) {
    m_methodMap[name] = func;
}

void browser::Browser::AddDOMContentLoadedHandler(std::function<void(Browser*)> func) {
    m_eventMethodMap["DOMContentLoaded"].emplace_back(func);
}

void browser::Browser::MoveWindow(Browser*, js_args_t jsArgs) {
    RECT rect;
    GetWindowRect(m_windowHandle, &rect);

    int newLeft = rect.left + jsArgs.Get<0, int>();
    int newTop = rect.top + jsArgs.Get<1, int>();
    int newWidth = rect.right - rect.left;
    int newHeight = rect.bottom - rect.top;

    SetWindowPos(m_windowHandle, nullptr, newLeft, newTop, newWidth, newHeight,
        SWP_NOZORDER | SWP_NOACTIVATE);
}

void browser::Browser::OnDomContentLoaded() {
    if (m_eventMethodMap.contains("DOMContentLoaded"))
        for (const auto& pFunction : m_eventMethodMap.at("DOMContentLoaded"))
            pFunction(this);
}