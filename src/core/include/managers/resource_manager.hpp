#pragma once

#ifndef __RESOURCE_MANAGER_HPP__
#define __RESOURCE_MANAGER_HPP__

#pragma warning(disable:4005)
#define CPPHTTPLIB_OPENSSL_SUPPORT
#pragma warning(default:4005)

#include <memory>
#include <string>
#include <httplib.h>

#include "../endpoint_mappers.hpp"
#include "manager.hpp"

class resource_manager : manager::abstract {
public:
    bool setup() override;
    bool shutdown() override;

    std::optional<int64_t> champion_name_to_id(const std::string& name);
    std::optional<std::string> champion_id_to_name(int64_t id);

    std::vector<std::string> champion_map_ids_to_names(const std::vector<int64_t> ids);
    std::vector<int64_t> champion_map_names_to_ids(const std::vector<std::string> ids);

private:
    datalocal::Resource m_data {};
    std::unique_ptr<httplib::Client> m_client = nullptr;
};

#endif // __RESOURCE_MANAGER_HPP__