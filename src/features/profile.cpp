#include "profile.hpp"

#include "../interface/holder.hpp"

#include "../ui/frame.hpp"
#include "../ui/label.hpp"

#include "../managers/browsermanager.hpp"
#include "../managers/connectormanager.hpp"

#undef interface

#include "../utils.hpp"
#include "../ui/button.hpp"
#include "../ui/checkbox.hpp"
#include "../ui/dropdown.hpp"
#include "../ui/selector.hpp"
#include "../ui/slider.hpp"
#include "../ui/input.hpp"

void feature::Profile::Setup(std::shared_ptr<ui::Frame> frame) {
    auto connectorManager = interface<ConnectorManager>::Get();
    auto browserManager = interface<BrowserManager>::Get();

    auto label = std::make_shared<ui::Label>("disconnected", "left");

    // auto connectionFrame = std::make_shared<ui::Frame>("connection", ui::FL_VERTICAL_AUTO);
    // connectionFrame->AddComponent<ui::Label>(label);
    // frame->AddComponent(std::move(frame));

    frame->AddComponent<ui::Label>(label);

    frame->AddComponent<ui::Button>(
        "clear tokens",
        ui::button_callback([this, connectorManager, browserManager]() { 
            auto result = connectorManager->MakeRequest(connector::request_type::POST, "/lol-challenges/v1/update-player-preferences/", "{\"challengeIds\": []}");
            if (result.status == 204)
                browserManager->CreateNotification("cleared", "the tokens have been cleared", notification_type::NONE);
            else
                browserManager->CreateNotification("failed to clear", "the tokens could no be cleared", notification_type::NONE);
        })
    );

    // frame->AddComponent<ui::Input>(
    //     "test input", "initial value", ui::input_callback([this](std::string newValue) {
    //         return newValue;
    //     })
    // );
    
    // frame->AddComponent<ui::Checkbox>(
    //     "test checkbox", true,
    //     ui::checkbox_callback([this](bool state) { return state; })
    // );

    // auto items = std::vector<std::string>{ "item1", "item2", "item3", "item4" };

    // frame->AddComponent<ui::Dropdown>(
    //     "test multi dropdown", true, std::vector<std::string>{ "item1" }, items,
    //     ui::dropdown_callback([this](std::string, bool, std::vector<std::string> list) {
    //         return list;
    //     })
    // );

    // frame->AddComponent<ui::Dropdown>(
    //     "test single dropdown", false, std::vector<std::string>{ "item1" }, items,
    //     ui::dropdown_callback([this](std::string, bool, std::vector<std::string> list) {
    //         return list;
    //     })
    // );

    // frame->AddComponent<ui::Selector>(
    //     "test Selector", 1, items,
    //     ui::selector_callback([this, items](std::string newMode) {
    //         const auto newModeIndex = (int)std::distance(items.begin(), std::ranges::find(items, newMode));
    //         return newModeIndex;
    //     })
    // );

    // frame->AddComponent<ui::Slider>(
    //     "test slider", 0, 100,ui::slider_callback([this](int newValue) {})
    // );

    connectorManager->AddConnectHandler(
        client_connect([this, browserManager, label]() {
            label->SetText("connected");
            label->Update(browserManager->GetHandle());
            browserManager->CreateNotification("connected", "the client has connected to league", notification_type::SUCCESS);
        })
    );

    connectorManager->AddDisconnectHandler(
        client_disconnect([this, browserManager, label]() {
            label->SetText("disconnected");
            label->Update(interface<BrowserManager>::Get()->GetHandle());
            browserManager->CreateNotification("disconnected", "the client has disconnected from league", notification_type::SUCCESS);
        })
    );
}

std::string feature::Profile::GetName() {
    return "profile";
}