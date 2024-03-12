#ifndef __CONNECTORMANAGER_HPP__
#define __CONNECTORMANAGER_HPP__

#include <map>
#include <string>
#include <vector>
#include <connector.hpp>

#include "../callbackholder.hpp"
#include "../ui/label.hpp"

typedef callback_holder<void, nlohmann::json> client_callback_t;

class connector_manager {
public:
    void Setup();
    void Shutdown();

    bool IsConnected();

    void AddEventListener(const std::string& endpoint, client_callback_t callback);
    connector::result_t MakeRequest(connector::request_type type, const std::string& endpoint, const std::string& data = "");

private:
    void ConnectHandler();
    void DisconnectHandler();

private:
    bool m_isConnected = false;
    std::shared_ptr<ui::label> m_connectionLabel;
};

#endif // __CONNECTORMANAGER_HPP__