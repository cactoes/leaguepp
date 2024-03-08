#include "connectormanager.hpp"

#include <iostream>
#include <connector.hpp>

void connector_manager::Setup() {
    connector::config_t connectorConfig = {};
    connectorConfig.enableWebSocketLogging = false;
    connectorConfig.connectHandler = std::bind(&connector_manager::ConnectHandler, this);
    connectorConfig.disconnectHandler = std::bind(&connector_manager::DisconnectHandler, this);

    connector::Connect(connectorConfig);
}

void connector_manager::Shutdown() {
    connector::Disconnect();
}

void connector_manager::ConnectHandler() {
    std::cout << "connected\n";
}

void connector_manager::DisconnectHandler() {
    std::cout << "disconnected\n";
}

bool connector_manager::IsConnected() {
    return m_isConnected;
}
