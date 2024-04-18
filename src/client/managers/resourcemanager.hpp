#ifndef __RESOURCEMANAGER_HPP__
#define __RESOURCEMANAGER_HPP__

#pragma warning(disable:4005)
#define CPPHTTPLIB_OPENSSL_SUPPORT
#pragma warning(default:4005)

#include <memory>
#include <string>
#include <httplib.h>

#include "../endpointmappers/datachampion.hpp"
#include "../endpointmappers/dataresource.hpp"

namespace data {
    struct Champion;
}; // namespace 

class ResourceManager {
public:
    bool Init();

    std::string ChampionIdToName(int id);
    int ChampionNameToId(const std::string& name);

private:
    bool GetLatestVersion(std::string& data);
    bool GetChampionData(const std::string& version, data::Champion& data);
    bool GetResourceData(datalocal::Resource& data);
    bool SaveResourceData(const datalocal::Resource& data);

private:
    datalocal::Resource m_data = {};
    std::unique_ptr<httplib::Client> m_client = nullptr;
};

#endif // __RESOURCEMANAGER_HPP__