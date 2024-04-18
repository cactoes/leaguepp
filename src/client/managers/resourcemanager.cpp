#include "resourcemanager.hpp"

#include <nlohmann/json.hpp>
#include <fstream>
#include <utils.hpp>
#include <ranges>
#include <algorithm>

bool ResourceManager::Init() {
    m_client = std::make_unique<httplib::Client>("https://ddragon.leagueoflegends.com");
    m_client->enable_server_certificate_verification(false);

    std::string version;
    if (!GetLatestVersion(version))
        return false;

    (void)GetResourceData(m_data);

    if (version == m_data.version)
        return true;

    m_data.version = version;

    data::Champion championData;
    if (!GetChampionData(version, championData))
        return false;

    m_data.data.clear();

    for (const auto& champion : championData.data.value()) {
        int id = -1;
        (void)ParseNumber(champion.second.key.value(), id);
        m_data.data.push_back(std::vector<datalocal::Datum>{ champion.second.name.value(), id });
    }

    return SaveResourceData(m_data);
}

std::string ResourceManager::ChampionIdToName(int id) {
    auto it = std::ranges::find_if(m_data.data, [id](const auto& obj) {
        return static_cast<int>(std::get<int64_t>(obj.at(1))) == id;
    });

    if (it == m_data.data.end())
        return "";

    return std::get<std::string>(it->at(0));
}

int ResourceManager::ChampionNameToId(const std::string& name) {
    auto it = std::ranges::find_if(m_data.data, [name](const auto& obj) {
        return StringCompareI(std::get<std::string>(obj.at(0)), name);
    });

    if (it == m_data.data.end())
        return 0;

    return static_cast<int>(std::get<int64_t>(it->at(1)));
}

bool ResourceManager::GetLatestVersion(std::string& data) {
    if (!m_client)
        return false;

    auto result = m_client->Get("/api/versions.json");

    if (result->status != 200)
        return false;

    nlohmann::json::parse(result->body).at(0).get_to<std::string>(data);
    return true;
}

bool ResourceManager::GetChampionData(const std::string& version, data::Champion& data) {
    if (!m_client)
        return false;

    auto result = m_client->Get("/cdn/" + version + "/data/en_US/champion.json");

    if (result->status != 200)
        return false;

    nlohmann::json::parse(result->body).get_to<data::Champion>(data);
    return true;
}

bool ResourceManager::GetResourceData(datalocal::Resource& data) {
    std::ifstream stream("resource.json", std::ios::binary);

    if (!stream.is_open())
        return false;

    nlohmann::json::parse(stream).get_to<datalocal::Resource>(data);
    return true;
}

bool ResourceManager::SaveResourceData(const datalocal::Resource& data) {
    std::ofstream stream("resource.json", std::ios::binary);

    if (!stream.is_open())
        return false;

    stream << nlohmann::json(data).dump();
    return true;
}