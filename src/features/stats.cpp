#include "stats.hpp"

#include "../interface/holder.hpp"

#include "../ui/frame.hpp"
#include "../ui/label.hpp"

#include "../managers/browsermanager.hpp"
#include "../managers/connectormanager.hpp"

#undef interface

void feature::Stats::Setup(std::shared_ptr<ui::Frame> frame) {
    auto connectorManager = interface<ConnectorManager>::Get();
    auto browserManager = interface<BrowserManager>::Get();

    auto label = std::make_shared<ui::Label>("-- disconnected --", "left");

    // auto connectionFrame = std::make_shared<ui::Frame>("connection", ui::FL_VERTICAL_AUTO);
    // connectionFrame->AddComponent<ui::Label>(label);
    // frame->AddComponent(std::move(frame));

    frame->AddComponent<ui::Label>(label);

    connectorManager->AddConnectHandler(
        client_connect([this, browserManager, label]() {
            label->SetText("-- connected --");
            label->Update(browserManager->GetHandle());
            browserManager->CreateNotification("connected", "the client has connected to league", notification_type::SUCCESS);
        })
    );

    connectorManager->AddDisconnectHandler(
        client_disconnect([this, browserManager, label]() {
            label->SetText("-- disconnected --");
            label->Update(interface<BrowserManager>::Get()->GetHandle());
            browserManager->CreateNotification("disconnected", "the client has disconnected from league", notification_type::SUCCESS);
        })
    );
}

std::string feature::Stats::GetName() {
    return "stats";
}