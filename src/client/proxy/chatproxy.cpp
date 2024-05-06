#include "chatproxy.hpp"

#include <utils.hpp>

extern "C" {
    bool __fastcall ChatProxyStartServer(const char* remote, unsigned int remotePort, unsigned int localPort);
    bool __fastcall ChatProxyStopServer();
    void __fastcall ChatProxyUseProxy(bool state);
}

void proxy::ChatProxy::Start() {
    SetProxyState(m_useProxy);

    m_thread = std::thread([this]() {
        (void)ChatProxyStartServer("tr1.chat.si.riotgames.com", 5223, m_chatPort);
    });
}

void proxy::ChatProxy::Stop() {
    (void)ChatProxyStopServer();
    // BUG: joining this thread can take a while maybe cus of the read locks in the dll
    // m_thread.join();
}

void proxy::ChatProxy::SetProxyState(bool useProxy) {
    m_useProxy = useProxy;
    ChatProxyUseProxy(m_useProxy);
}
