#include <iostream>
#include <connector.hpp>

#include "browser/browser.hpp"

#include "ui/frame.hpp"
#include "ui/button.hpp"
#include "ui/label.hpp"
#include "ui/checkbox.hpp"
#include "ui/slider.hpp"
#include "ui/dropdown.hpp"
#include "ui/selector.hpp"

void OnToggleCheckBox1(bool state) {
    std::cout << "switched state to: " << state << "\n";
}

class testClass {
public:
    void OnClickButton() {
        std::cout << "onclick\n";
    }

    void OnSliderChange(int value) {
        std::cout << "new slider state: " << value << "\n";
    }

    void OnDropdownChange(std::string item, bool newItemState, std::vector<std::string> list) {
        std::cout << "what: " << item << "\n";
        std::cout << "newState: " << newItemState << "\n";

        for (auto& it : list) {
            std::cout << "item: " << it << "\n";
        }
    }

    void SelectorChange(std::string current) {
         std::cout << "current: " << current << "\n";
    }
};

void GetMainLayout(browser::browser* handle, browser::js_args_t) {
    static testClass g_tc{};

    static ui::frame mainFrame = ui::frame("frame name", ui::FL_VERTICAL, FRAME_TARGET_MAIN);
    static ui::frame frame2 = ui::frame("", ui::FL_HORIZONTAL, FRAME_TARGET_MAIN);

    mainFrame.AddComponent<ui::button>(std::make_unique<ui::button>("btn", ui::button_callback_t(&testClass::OnClickButton, &g_tc)));
    mainFrame.AddComponent<ui::label>("test label");
    mainFrame.AddComponent<ui::checkbox>("checkbox", ui::checkbox_callback_t(&OnToggleCheckBox1));
    mainFrame.AddComponent<ui::slider>("slider", 0, 100, ui::slider_callback_t(&testClass::OnSliderChange, &g_tc));
    mainFrame.AddComponent<ui::dropdown>("dropdown", std::vector<std::string>{ "opt1", "opt2", "opt3" }, ui::dropdown_callback_t(&testClass::OnDropdownChange, &g_tc));
    mainFrame.AddComponent<ui::selector>("selector", std::vector<std::string>{ "opt1", "opt2", "opt3" }, ui::selector_callback_t(&testClass::SelectorChange, &g_tc));

    mainFrame.Register(handle);
    frame2.Register(handle);
}

void HandleWindowEvent(browser::browser* browser, browser::js_args_t args) {
    if (args.Size() < 1)
        return;

    const auto event = args.Get<0, std::string>();

    if (event == "minimize") {
        ShowWindow(browser->m_windowHandle, SW_MINIMIZE);
        return;
    }

    if (event == "resize") {
        WINDOWPLACEMENT wp;
        wp.length = sizeof(WINDOWPLACEMENT);
        if (GetWindowPlacement(browser->m_windowHandle, &wp))
            ShowWindow(browser->m_windowHandle, wp.showCmd != SW_SHOWMAXIMIZED ? SW_SHOWMAXIMIZED : SW_RESTORE);

        return;
    }

    if (event == "close") {
        PostQuitMessage(0);
        return;
    }
}

int main() {
    browser::window_config_t windowConfig = {};
    windowConfig.name = "league++";
    windowConfig.flags = browser::window_flags::WF_SHOW | browser::window_flags::WF_HIDE_TITLE_BAR;

    browser::browser_config_t browserConfig = {};
    browserConfig.flags = browser::browser_flags::BF_ALLOW_DEV_TOOLS;
#ifdef _DEBUG
    SET_FLAG(browserConfig.flags, browser::browser_flags::BF_ENABLE_DEBUG_CONSOLE);
#endif

    auto browserHandle = browser::CreateBrowser(browserConfig, windowConfig);

    connector::config_t connectorConfig = {};
    connectorConfig.enableWebSocketLogging = false;
    // connectorConfig.connectHandler = &ConnectHandler;
    // connectorConfig.disconnectHandler = &DisconnectHandler;

    connector::Connect(connectorConfig);

    browserHandle->RegisterFunction(CREATE_REGISTRATION(HandleWindowEvent));

    browserHandle->RegisterFunction(CREATE_REGISTRATION(GetMainLayout));

    browserHandle->Start();

    connector::Disconnect();

    return 0;
}