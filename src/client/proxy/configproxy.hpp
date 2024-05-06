#ifndef __CONFIGPROXY_HPP__
#define __CONFIGPROXY_HPP__

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

#include <thread>

namespace proxy {
    class ConfigProxy {
    public:
        ConfigProxy(int configPort, int chatPort)
            : m_configPort(configPort), m_chatPort(chatPort) {}

        void Start();
        void Stop();

        void OnRequest(const httplib::Request& req, httplib::Response& res);

    private:
        int m_configPort;
        int m_chatPort;
        httplib::Server m_server;
        std::thread m_serverThread;
    };
}; // namespace proxy

#endif // __CONFIGPROXY_HPP__