#ifndef __CONNECTORMANAGER_HPP__
#define __CONNECTORMANAGER_HPP__

#include "../callbackholder.hpp"
#include "../ui/label.hpp"

#undef interface

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

    std::shared_ptr<ui::label> m_connectionLabel;
};

#endif // __CONNECTORMANAGER_HPP__