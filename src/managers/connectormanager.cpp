#include "connectormanager.hpp"

#include "../interface/holder.hpp"
#include "browsermanager.hpp"
#include "layoutmanager.hpp"

void connector_manager::Setup() {
    connector::config_t connectorConfig = {};
    connectorConfig.enableWebSocketLogging = false;
    connectorConfig.connectHandler = std::bind(&connector_manager::ConnectHandler, this);
    connectorConfig.disconnectHandler = std::bind(&connector_manager::DisconnectHandler, this);

    // persistent components
    m_connectionLabel = std::make_shared<ui::label>("-- disconnected --", "left");

    // custom frame
    auto frame = std::make_shared<ui::frame>("connection", ui::FL_VERTICAL_AUTO);
    frame->AddComponent<ui::label>(m_connectionLabel);

    // bind to stats frame
    interface::GetHolder<layout_manager>(from_singleton)
        ->GetFrame()
        ->GetComponent<ui::frame>("StatsFrame")
        ->AddComponent(std::move(frame));

    connector::Connect(connectorConfig);
}

void connector_manager::Shutdown() {
    connector::Disconnect();
}

void connector_manager::ConnectHandler() {
    auto browserManager = interface::GetHolder<browser_manager>(from_singleton);
    m_connectionLabel->SetText("-- connected --");
    m_connectionLabel->Update(browserManager->GetHandle());
    browserManager->CreateNotification("connected", "the client has connected to league", notification_type::SUCCESS);
}

void connector_manager::DisconnectHandler() {
    auto browserManager = interface::GetHolder<browser_manager>(from_singleton);
    m_connectionLabel->SetText("-- disconnected --");
    m_connectionLabel->Update(interface::GetHolder<browser_manager>(from_singleton)->GetHandle());
    browserManager->CreateNotification("disconnected", "the client has disconnected from league", notification_type::SUCCESS);
}

bool connector_manager::IsConnected() {
    return m_isConnected;
}

void connector_manager::AddEventListener(const std::string& endpoint, client_callback_t callback) {
    connector::AddEventHandler(endpoint, callback.GetCallback());
}

connector::result_t connector_manager::MakeRequest(connector::request_type type, const std::string& endpoint, const std::string& data) {
    return connector::MakeRequest(type, endpoint, data);
}