#include <fstream>

#include "managers/resource_manager.hpp"
#include "utils.hpp"

bool resource_manager::setup() {
    m_client = std::make_unique<httplib::Client>("https://ddragon.leagueoflegends.com");
    m_client->enable_server_certificate_verification(false);

    // get latest version of riot api
    auto result = m_client->Get("/api/versions.json");
    if (!result || result->status != 200)
        return false;

    std::string version = nlohmann::json::parse(result->body).at(0).get<std::string>();

    // load our local resource file if it exists
    std::ifstream in_stream("client_resource.json", std::ios::binary);
    if (in_stream.is_open()) {
        nlohmann::json::parse(in_stream).get_to<datalocal::Resource>(m_data);
        in_stream.close();
    }

    // data is still up to date yy
    if (version == m_data.version)
        return true;

    // get the chamption data from riot
    // FIXME: lang is always en_US
    result = m_client->Get("/cdn/" + version + "/data/en_US/champion.json");
    if (!result || result->status != 200)
        return false;

    data::Champion champion_data = nlohmann::json::parse(result->body).get<data::Champion>();

    m_data.data.clear();
    m_data.data.reserve(champion_data.data->size());

    for (const auto& champion : champion_data.data.value()) {
        auto id = lpp_core_utils::parse_number<int64_t>(champion.second.key.value());
        m_data.data.push_back(
            std::array<datalocal::Datum, 2>{ champion.second.name.value(), id.value_or(-1) }
        );
    }

    std::ofstream out_stream("client_resource.json", std::ios::binary);
    out_stream << nlohmann::json(m_data).dump();

    return true;
}

std::optional<int64_t> resource_manager::champion_name_to_id(const std::string& name) {
    auto it = std::ranges::find_if(m_data.data, [&name](const auto& obj) {
        return lpp_core_utils::strcmpi(std::get<std::string>(obj.at(0)), name);
    });

    if (it != m_data.data.end())
        return std::get<int64_t>(it->at(1));

    return std::nullopt;
}

std::optional<std::string> resource_manager::champion_id_to_name(int64_t id) {
    auto it = std::ranges::find_if(m_data.data, [id](const auto& obj) {
        return std::get<int64_t>(obj.at(1)) == id;
    });

    if (it != m_data.data.end())
        return std::get<std::string>(it->at(0));

    return std::nullopt;
}

std::vector<std::string> resource_manager::champion_map_ids_to_names(const std::vector<int64_t> ids) {
    std::vector<std::string> out {};
    out.reserve(ids.size());
    for (const auto& id : ids)
        if (auto result = champion_id_to_name(id); result.has_value())
            out.push_back(result.value());

    return out;
}

std::vector<int64_t> resource_manager::champion_map_names_to_ids(const std::vector<std::string> ids) {
    std::vector<int64_t> out {};
    out.reserve(ids.size());
    for (const auto& id : ids)
        if (auto result = champion_name_to_id(id); result.has_value())
            out.push_back(result.value());

    return out;
}

bool resource_manager::shutdown() {
    m_client->stop();
    m_client = nullptr;
    return true;
}