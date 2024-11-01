#include "managers/league_connector_manager.hpp"

#include <iostream>

bool league_connector_manager::setup() {
    if (m_is_setup)
        return false;

    connector::config_t config{};
    config.connectHandler = [this]() { for (auto& handler : m_connect_handlers) handler(); };
    config.disconnectHandler = [this]() { for (auto& handler : m_disconnect_handlers) handler(); };
    connector::Connect(config);

    m_is_setup = true;
    return true;
}

void league_connector_manager::add_connect_handler(const ru_function<void>& callback) {
    m_connect_handlers.push_back(callback);
}

void league_connector_manager::add_disconnect_handler(const ru_function<void>& callback) {
    m_disconnect_handlers.push_back(callback);
}

bool league_connector_manager::shutdown() {
    if (!m_is_setup)
        return false;

    connector::Disconnect();
    m_is_setup = false;
    return true;
}

void league_connector_manager::add_endpoint_callback(const std::string& endpoint, ru_function<void, std::string, nlohmann::json> callback) {
    connector::AddEventHandler(endpoint, callback.get().value());
}