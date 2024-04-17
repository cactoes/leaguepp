#include "profile.hpp"

#include "../interface/holder.hpp"

#include "../managers/connectormanager.hpp"

#include "endpointmappers/summaryplayerdata.hpp"

#undef interface

void feature::Profile::Setup(std::shared_ptr<ui::Frame> frame, IUiFramework* frameworkApiHandle) {
    auto connectorManager = interface<ConnectorManager>::Get();

    auto label = frame->AddLabel("disconnected", "left");

    frame->AddButton(
        "clear tokens",
        ui::button_callback([this, connectorManager, frameworkApiHandle]() {
            auto playerDataResult = connectorManager->MakeRequest(connector::request_type::GET, "/lol-challenges/v1/summary-player-data/local-player");

            if (playerDataResult.status == 200) {
                auto data = playerDataResult.data.get<challenges::SummaryPlayerData>();

                const auto& bannerId = data.bannerId.value();
                const auto& titleId = data.title->itemId.value();

                auto result = connectorManager->MakeRequest(connector::request_type::POST, "/lol-challenges/v1/update-player-preferences",
                    "{\"bannerAccent\":\"" + bannerId + "\",\"challengeIds\":[],\"title\":\"" + std::to_string(titleId) + "\"}");

                if (result.status == 204) {
                    frameworkApiHandle->CreateNotification("cleared", "the tokens have been cleared");
                    return;
                }
            }

            frameworkApiHandle->CreateNotification("failed to clear", "the tokens could no be cleared");
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
    //     "test slider", 0, 100,ui::slider_callback([this](int) {})
    // );

    connectorManager->AddConnectHandler(
        client_connect([this, label, frameworkApiHandle]() {
            label->SetText("connected");
            label->Update();
            frameworkApiHandle->CreateNotification("connected", "the client has connected to league");
        })
    );

    connectorManager->AddDisconnectHandler(
        client_disconnect([this, label, frameworkApiHandle]() {
            label->SetText("disconnected");
            label->Update();
            frameworkApiHandle->CreateNotification("disconnected", "the client has disconnected from league");
        })
    );
}

std::string feature::Profile::GetName() {
    return "profile";
}