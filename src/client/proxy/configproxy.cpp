#include "configproxy.hpp"

#include <utils.hpp>
#include <nlohmann/json.hpp>

void proxy::ConfigProxy::Start() {
    m_server.Get(".*", [this](const httplib::Request& req, httplib::Response& res) { OnRequest(req, res); });

    m_serverThread = std::thread([this]() {
        m_server.listen("127.0.0.1", m_configPort);
    });
}

void proxy::ConfigProxy::Stop() {
    m_server.stop();
    m_serverThread.join();
}

void proxy::ConfigProxy::OnRequest(const httplib::Request& req, httplib::Response& res) {
    httplib::Client client("https://clientconfig.rpg.riotgames.com");
    httplib::Result result = client.Get(req.target, httplib::Headers{
        { "authorization", req.get_header_value("authorization") },
        { "user-agent", req.get_header_value("user-agent") },
        { "x-riot-entitlements-jwt", req.get_header_value("x-riot-entitlements-jwt") }
    });

    nlohmann::json content = nlohmann::json::parse(result->body);

    if (content.contains("chat.host"))
        content["chat.host"] = "127.0.0.1";

    if (content.contains("chat.port"))
        content["chat.port"] = m_chatPort;

    if (content.contains("chat.affinities"))
        for (const auto& k : content["chat.affinities"].items())
            content["chat.affinities"][k.key()] = "127.0.0.1";

    if (content.contains("chat.allow_bad_cert.enabled"))
        content["chat.allow_bad_cert.enabled"] = true;

    res.set_content(content.dump(), "application/json");
}