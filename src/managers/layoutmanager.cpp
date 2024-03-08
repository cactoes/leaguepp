#include "layoutmanager.hpp"

void layout_manager::Setup(browser::browser* handle) {
    handle->RegisterFunction("GetMainLayout", CREATE_REGISTRATION_MEMBER(layout_manager::PushLayout));

    m_frameMain.AddComponent<ui::label>("test label");
}

void layout_manager::PushLayout(browser::browser* handle, browser::js_args_t args) {
    m_frameMain.Register(handle);
}

// void OnToggleCheckBox1(bool state) {
//     std::cout << "switched state to: " << state << "\n";
// }

// class testClass {
// public:
//     void OnClickButton() {
//         std::cout << "onclick\n";
//     }

//     void OnSliderChange(int value) {
//         std::cout << "new slider state: " << value << "\n";
//     }

//     void OnDropdownChange(std::string item, bool newItemState, std::vector<std::string> list) {
//         std::cout << "what: " << item << "\n";
//         std::cout << "newState: " << newItemState << "\n";

//         for (auto& it : list) {
//             std::cout << "item: " << it << "\n";
//         }
//     }

//     void SelectorChange(std::string current) {
//          std::cout << "current: " << current << "\n";
//     }
// };

// void GetMainLayout(browser::browser* handle, browser::js_args_t) {
//     static testClass g_tc{};

//     static ui::frame mainFrame = ui::frame("frame name", ui::FL_VERTICAL, FRAME_TARGET_MAIN);
//     static ui::frame frame2 = ui::frame("", ui::FL_HORIZONTAL, FRAME_TARGET_MAIN);

//     mainFrame.AddComponent<ui::button>(std::make_unique<ui::button>("btn", ui::button_callback_t(&testClass::OnClickButton, &g_tc)));
//     mainFrame.AddComponent<ui::label>("test label");
//     mainFrame.AddComponent<ui::checkbox>("checkbox", ui::checkbox_callback_t(&OnToggleCheckBox1));
//     mainFrame.AddComponent<ui::slider>("slider", 0, 100, ui::slider_callback_t(&testClass::OnSliderChange, &g_tc));
//     mainFrame.AddComponent<ui::dropdown>("dropdown", std::vector<std::string>{ "opt1", "opt2", "opt3" }, ui::dropdown_callback_t(&testClass::OnDropdownChange, &g_tc));
//     mainFrame.AddComponent<ui::selector>("selector", std::vector<std::string>{ "opt1", "opt2", "opt3" }, ui::selector_callback_t(&testClass::SelectorChange, &g_tc));

//     mainFrame.Register(handle);
//     frame2.Register(handle);
// }
