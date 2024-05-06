#ifndef __CHATPROXY_HPP__
#define __CHATPROXY_HPP__

#include <thread>

namespace proxy {
    class ChatProxy {
    public:
        ChatProxy(int chatPort) : m_chatPort(chatPort) {}

        void Start();
        void Stop();
        void SetProxyState(bool useProxy);

    private:
        bool m_useProxy;
        int m_chatPort;
        std::thread m_thread;
    };
}; // namespace proxy

#endif // __CHATPROXY_HPP__