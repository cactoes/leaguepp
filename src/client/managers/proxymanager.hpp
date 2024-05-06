#ifndef __PROXYMANAGER_HPP__
#define __PROXYMANAGER_HPP__

#include <memory>
#include <framework.hpp>

#include "../proxy/configproxy.hpp"
#include "../proxy/chatproxy.hpp"

#define PROXY_PORT_CONFIG 6041
#define PROXY_PORT_CHAT 6042

class ProxyManager {
public:
    bool Init(IUiFramework* frameworkApiHandle);
    void Shutdown();

    void UpdateLabel(bool state);

private:
    void SetupUi(IUiFramework* frameworkApiHandle);

private:
    std::unique_ptr<proxy::ConfigProxy> m_configProxy = nullptr;
    std::unique_ptr<proxy::ChatProxy> m_chatProxy = nullptr;
    std::shared_ptr<ui::Label> m_label = nullptr;
};

#endif // __PROXYMANAGER_HPP__