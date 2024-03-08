#ifndef __CONNECTORMANAGER_HPP__
#define __CONNECTORMANAGER_HPP__

#include "../callbackholder.hpp"

class connector_manager {
public:
    void Setup();
    void Shutdown();

    bool IsConnected();

private:
    void ConnectHandler();
    void DisconnectHandler();

private:
    bool m_isConnected = false;
};

#endif // __CONNECTORMANAGER_HPP__