#include "configmanager.hpp"

#include <fstream>

template <>
inline char* Cvar<std::string>::Data() {
    return m_value.data();
}

template <>
inline uint64_t Cvar<std::string>::Size() {
    return m_value.size();
}

template <>
inline void Cvar<std::string>::SetData(char* data, size_t size) {
    m_value.assign(data, data + size);
}

template <>
inline char* Cvar<std::vector<int>>::Data() {
    return reinterpret_cast<char*>(m_value.data());
}

template <>
inline void Cvar<std::vector<int>>::SetData(char* data, size_t size) {
    m_value.insert(m_value.begin(), (int*)data, (int*)data + (size / sizeof(int)));
}

template <>
inline uint64_t Cvar<std::vector<int>>::Size() {
    return sizeof(int) * m_value.size();
}

template <>
inline char* Cvar<std::vector<float>>::Data() {
    return reinterpret_cast<char*>(m_value.data());
}

template <>
inline void Cvar<std::vector<float>>::SetData(char* data, size_t size) {
    m_value.insert(m_value.begin(), (float*)data, (float*)data + (size / sizeof(float)));
}

template <>
inline uint64_t Cvar<std::vector<float>>::Size() {
    return sizeof(float) * m_value.size();
}

bool Config::HasVar(const char* key) {
    return HasVar(hash::Fnv1a64HashConst(key));
}

bool Config::HasVar(uint64_t key) {
    return m_data.contains(key);
}

Config::data_t Config::Data() {
    data_t data = {};

    for (auto& [k, v] : m_data) {
        if (v == nullptr)
            continue;

        // sizeof key
        data.size += sizeof(k);
        // sizeof size
        data.size += sizeof(uint64_t);
        // sizeof data
        data.size += v->Size();
    }

    data.data = std::make_unique<char[]>(data.size);
    uint64_t writePtr = 0;

    for (auto& [k, v] : m_data) {
        if (v == nullptr)
            continue;
            
        // write key
        memcpy(data.data.get() + writePtr, (char*)&k, sizeof(k));
        writePtr += sizeof(k);

        // write size
        auto size = v->Size();
        memcpy(data.data.get() + writePtr, (char*)&size, sizeof(uint64_t));
        writePtr += sizeof(uint64_t);
        
        // write data
        memcpy(data.data.get() + writePtr, v->Data(), v->Size());
        writePtr += size;
    }

    return data;
}

bool ConfigManager::Init() {
    auto cfg = Create(CONFIG_BASIC);
    cfg->AddTemplate<bool>("lobby::bAutoAccept");
    cfg->AddTemplate<bool>("lobby::bAutoDodge");
    cfg->AddTemplate<std::vector<int>>("lobby::autoDodgeLanes");
    
    cfg->AddTemplate<std::string>("profile::sTier");
    cfg->AddTemplate<std::string>("profile::sDivision");
    cfg->AddTemplate<std::string>("profile::sMastery");
    cfg->AddTemplate<std::string>("profile::bAutoSet");

    cfg->AddTemplate<bool>("autoPicker::bEnabled");
    cfg->AddTemplate<int>("autoPicker::nMode");
    cfg->AddTemplate<int>("autoPicker::nStrictness");
    cfg->AddTemplate<std::string>("autoPicker::sPreferredLineBlind");
    cfg->AddTemplate<bool>("autoPicker::bEarlyDeclare");

    for (const auto& lane : std::vector<std::string>{ "top", "jungle", "middle", "bottom", "utility" }) {
        cfg->AddTemplate<std::vector<int>>(("autoPicker::" + lane + "::banIds").c_str());
        cfg->AddTemplate<std::vector<int>>(("autoPicker::" + lane + "::pickIds").c_str());
    }

    if (!Load(cfg))
        Dump(cfg);

    return true;
}

bool ConfigManager::Dump(config_handle config) {
    std::ofstream stream(config->m_name + ".bin", std::ios::binary);
    auto data = config->Data();
    stream.write(data.data.get(), data.size);

    return true;
}

config_handle ConfigManager::Create(const std::string& name) {
    if (m_configs.contains(name))
        return nullptr;

    m_configs[name] = std::make_shared<Config>(name);
    return Get(name);
}

config_handle ConfigManager::Get(const std::string& name) {
    return m_configs.at(name);
}

bool ConfigManager::Load(config_handle config) {
    std::ifstream stream(config->m_name + ".bin", std::ios::binary);

    if (!stream.is_open() || stream.peek() == EOF)
        return false;

    while (stream.peek() != EOF) {
        uint64_t hash = 0;
        stream.read(reinterpret_cast<char*>(&hash), sizeof(hash));

        uint64_t size = 0;
        stream.read(reinterpret_cast<char*>(&size), sizeof(size));

        auto data = std::make_unique<char[]>(size);

        if (config->HasVar(hash)) {
            stream.read(data.get(), size);
            config->m_data.at(hash)->SetData(data.get(), size);
        } else {
            stream.ignore(size);
        }

        // memory alignment is not needed ww
        // const auto blocksize = sizeof(hash) + sizeof(size) + size;
        // if ((blocksize % 8) != 0 && (blocksize % 2) != 0)
        //     stream.ignore(8 - (blocksize % 8));
    }

    return true;
}