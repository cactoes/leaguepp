#ifndef __CONNECTORMANAGER_HPP__
#define __CONNECTORMANAGER_HPP__

#include <memory>
#include <connector.hpp>

#include "../callbackholder.hpp"

namespace ui {
    class Label;
}; // namespace ui

typedef callback_holder<void, std::string, nlohmann::json> client_callback_t;

class ConnectorManager {
public:
    bool Init();
    void Shutdown();

    bool IsConnected();

    void AddEventListener(const std::string& endpoint, client_callback_t callback);
    connector::result_t MakeRequest(connector::request_type type, const std::string& endpoint, const std::string& data = "");

private:
    void ConnectHandler();
    void DisconnectHandler();

private:
    bool m_isConnected = false;
    std::shared_ptr<ui::Label> m_connectionLabel;
};

#endif // __CONNECTORMANAGER_HPP__