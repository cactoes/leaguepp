#include "connectormanager.hpp"

#include <connector.hpp>

#include "../interface/holder.hpp"

#undef interface

bool ConnectorManager::Init() {
    connector::config_t connectorConfig = {};
    connectorConfig.enableWebSocketLogging = false;
    connectorConfig.connectHandler = std::bind(&ConnectorManager::ConnectHandler, this);
    connectorConfig.disconnectHandler = std::bind(&ConnectorManager::DisconnectHandler, this);

    connector::Connect(connectorConfig);
    return true;
}

void ConnectorManager::Shutdown() {
    connector::Disconnect();
}

void ConnectorManager::ConnectHandler() {
    for (auto& handler : m_connectHandlers)
        handler.Run();
}

void ConnectorManager::DisconnectHandler() {
    for (auto& handler : m_disconnectHandlers)
        handler.Run();
}

bool ConnectorManager::IsConnected() {
    return m_isConnected;
}

void ConnectorManager::AddConnectHandler(client_connect callback) {
    m_connectHandlers.push_back(callback);
}

void ConnectorManager::AddDisconnectHandler(client_disconnect callback) {
    m_disconnectHandlers.push_back(callback);
}

void ConnectorManager::AddEventListener(const std::string& endpoint, client_callback callback) {
    connector::AddEventHandler(endpoint, callback.GetCallback());
}

connector::result_t ConnectorManager::MakeRequest(connector::request_type type, const std::string& endpoint, const std::string& data) {
    return connector::MakeRequest(type, endpoint, data);
}