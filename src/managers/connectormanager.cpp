#include "connectormanager.hpp"

#include <connector.hpp>

#include "../interface/holder.hpp"
#include "../ui/label.hpp"
#include "../ui/frame.hpp"
#include "browsermanager.hpp"
#include "layoutmanager.hpp"

#undef interface

bool ConnectorManager::Init() {
    connector::config_t connectorConfig = {};
    connectorConfig.enableWebSocketLogging = false;
    connectorConfig.connectHandler = std::bind(&ConnectorManager::ConnectHandler, this);
    connectorConfig.disconnectHandler = std::bind(&ConnectorManager::DisconnectHandler, this);

    // persistent components
    m_connectionLabel = std::make_shared<ui::Label>("-- disconnected --", "left");

    // custom frame
    auto frame = std::make_shared<ui::Frame>("connection", ui::FL_VERTICAL_AUTO);
    frame->AddComponent<ui::Label>(m_connectionLabel);

    // bind to stats frame
    interface<LayoutManager>::Get()
        ->GetFrame()
        ->GetComponent<ui::Frame>("StatsFrame")
        ->AddComponent(std::move(frame));

    connector::Connect(connectorConfig);
    return true;
}

void ConnectorManager::Shutdown() {
    connector::Disconnect();
}

void ConnectorManager::ConnectHandler() {
    auto browserManager = interface<browser_manager>::Get();
    m_connectionLabel->SetText("-- connected --");
    m_connectionLabel->Update(browserManager->GetHandle());
    browserManager->CreateNotification("connected", "the client has connected to league", notification_type::SUCCESS);
}

void ConnectorManager::DisconnectHandler() {
    auto browserManager = interface<browser_manager>::Get();
    m_connectionLabel->SetText("-- disconnected --");
    m_connectionLabel->Update(interface<browser_manager>::Get()->GetHandle());
    browserManager->CreateNotification("disconnected", "the client has disconnected from league", notification_type::SUCCESS);
}

bool ConnectorManager::IsConnected() {
    return m_isConnected;
}

void ConnectorManager::AddEventListener(const std::string& endpoint, client_callback_t callback) {
    connector::AddEventHandler(endpoint, callback.GetCallback());
}

connector::result_t ConnectorManager::MakeRequest(connector::request_type type, const std::string& endpoint, const std::string& data) {
    return connector::MakeRequest(type, endpoint, data);
}