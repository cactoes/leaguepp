#include "connectormanager.hpp"

#include <iostream>
#include <connector.hpp>

#include "../interface/holder.hpp"
#include "browsermanager.hpp"
#include "layoutmanager.hpp"

void connector_manager::Setup() {
    connector::config_t connectorConfig = {};
    connectorConfig.enableWebSocketLogging = false;
    connectorConfig.connectHandler = std::bind(&connector_manager::ConnectHandler, this);
    connectorConfig.disconnectHandler = std::bind(&connector_manager::DisconnectHandler, this);

    // persistent components
    m_connectionLabel = std::make_shared<ui::label>("-- disconnected --");

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
    m_connectionLabel->SetText("-- connected --");
    m_connectionLabel->Update(interface::GetHolder<browser_manager>(from_singleton)->GetHandle());
}

void connector_manager::DisconnectHandler() {
    m_connectionLabel->SetText("-- disconnected --");
    m_connectionLabel->Update(interface::GetHolder<browser_manager>(from_singleton)->GetHandle());
}

bool connector_manager::IsConnected() {
    return m_isConnected;
}