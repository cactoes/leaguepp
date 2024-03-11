#include "configmanager.hpp"

#include <fstream>

template <>
inline char* cvar<std::string>::Data() {
    return m_value.data();
}

template <>
inline uint64_t cvar<std::string>::Size() {
    return m_value.size();
}

template <>
inline char* cvar<std::vector<int>>::Data() {
    return reinterpret_cast<char*>(m_value.data());
}

template <>
inline uint64_t cvar<std::vector<int>>::Size() {
    return sizeof(int) * m_value.size();
}

template <>
inline char* cvar<std::vector<float>>::Data() {
    return reinterpret_cast<char*>(m_value.data());
}

template <>
inline uint64_t cvar<std::vector<float>>::Size() {
    return sizeof(float) * m_value.size();
}

bool config::HasVar(const char* key) {
    return HasVar(hash::Fnv1a64HashConst(key));
}

bool config::HasVar(uint64_t key) {
    return m_data.contains(key);
}

config::data_t config::Data() {
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

void config_manager::Setup() {
    auto cfg = CreateConfig("cfg");
    cfg->AddTemplate<bool>("lobby::autoAccept");
    cfg->AddTemplate<int>("bot::mode");
    cfg->AddTemplate<int>("bot::strictness");

    if (!LoadConfig(cfg))
        DumpConfig(cfg);
}

void config_manager::DumpConfig(std::shared_ptr<config> config) {
    std::ofstream stream(config->m_name + ".bin", std::ios::binary);
    auto data = config->Data();
    stream.write(data.data.get(), data.size);
}

std::shared_ptr<config> config_manager::CreateConfig(const std::string& name) {
    if (m_configs.contains(name))
        return nullptr;

    m_configs[name] = std::make_shared<config>(name);
    return GetConfig(name);
}

std::shared_ptr<config> config_manager::GetConfig(const std::string& name) {
    return m_configs.at(name);
}

bool config_manager::LoadConfig(std::shared_ptr<config> config) {
    std::ifstream stream(config->m_name + ".bin", std::ios::binary);

    if (!stream.is_open() || stream.peek() == EOF)
        return false;

    while (stream.peek() != EOF) {
        uint64_t hash = 0;
        stream.read(reinterpret_cast<char*>(&hash), sizeof(hash));

        uint64_t size = 0;
        stream.read(reinterpret_cast<char*>(&size), sizeof(size));

        if (config->HasVar(hash)) {
            stream.read(config->m_data.at(hash)->Data(), sizeof(size));
        } else {
            stream.ignore(size);
        }

        const auto blocksize = sizeof(hash) + sizeof(size) + size;
        if ((blocksize % 8) != 0 && (blocksize % 2) != 0) {
            stream.ignore(8 - (blocksize % 8));
        }
    }

    return true;
}