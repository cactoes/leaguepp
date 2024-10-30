#include "managers/league_connector_manager.hpp"

#include <iostream>

bool league_connector_manager::setup() {
    if (m_is_setup)
        return false;

    connector::settings_t settings{};
    m_connector = std::make_unique<connector::connector>(settings);
    m_connector->set_connect_handler([this]() { for (auto& handler : m_connect_handlers) handler(); });
    m_connector->set_disconnect_handler([this]() { for (auto& handler : m_disconnect_handlers) handler(); });

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

    m_connector->disconnect();
    m_is_setup = false;
    return true;
}

void league_connector_manager::add_endpoint_callback(const std::string& endpoint, ru_function<void, std::string, nlohmann::json> callback) {
    m_connector->add_event_listener(endpoint, callback.get().value());
}