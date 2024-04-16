#ifndef __CONNECTORMANAGER_HPP__
#define __CONNECTORMANAGER_HPP__

#include <memory>
#include <connector.hpp>
#include <vector>

#include "../callbackholder.hpp"

typedef callback_holder<void, std::string, nlohmann::json> client_callback;
typedef callback_holder<void> client_connect;
typedef callback_holder<void> client_disconnect;

class ConnectorManager {
public:
    bool Init();
    void Shutdown();

    bool IsConnected();

    void AddConnectHandler(client_connect callback);
    void AddDisconnectHandler(client_disconnect callback);

    void AddEventListener(const std::string& endpoint, client_callback callback);
    connector::result_t MakeRequest(connector::request_type type, const std::string& endpoint, const std::string& data = "");

private:
    void ConnectHandler();
    void DisconnectHandler();

private:
    bool m_isConnected = false;
    std::vector<client_connect> m_connectHandlers = {};
    std::vector<client_disconnect> m_disconnectHandlers = {};
};

#endif // __CONNECTORMANAGER_HPP__