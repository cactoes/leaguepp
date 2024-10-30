#include "managers/config_manager.hpp"

bool config_manager::load_config(config_handle config) {
    std::ifstream stream(config->m_name + ".lcf", std::ios::binary | std::ios::ate);

    if (!stream.is_open())
        return false;

    std::streamsize size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    config::config_data_raw_t config_data_raw {};
    config_data_raw.size = size;
    config_data_raw.data = std::make_unique<uint8_t[]>(config_data_raw.size);
    stream.read((char*)config_data_raw.data.get(), config_data_raw.size);

    // TODO: notify user: import fails if version miss matches
    return config->import(config_data_raw);
}

bool config_manager::dump_config(config_handle config) {
    std::ofstream stream(config->m_name + ".lcf", std::ios::binary);

    auto config_data_raw = config->dump();
    stream.write((char*)config_data_raw.data.get(), config_data_raw.size);

    return true;
}

config_handle config_manager::get_config(const std::string& name) {
    return m_configs.contains(name) ? m_configs.at(name) : nullptr;
}

bool config_manager::save_config(config_handle config) {
    if (m_configs.contains(config->m_name))
        return false;

    m_configs[config->m_name] = config;
    return true;
}

config_handle config_manager::create_config(const std::string& name) {
    if (!save_config(std::make_shared<config>(name)))
        return nullptr;

    return get_config(name);
}

config::config_data_raw_t config::dump() const {
    config_data_raw_t config_data_raw {};

    for (auto& [ key, value ] : m_cvars) {
        if (value == nullptr)
            continue;

        // key, m_data_size, m_data
        config_data_raw.size += sizeof(key) + sizeof(size_t) + value->__get_size();
    }

    config_data_raw.size += sizeof(m_config_version);
    config_data_raw.data = std::make_unique<uint8_t[]>(config_data_raw.size);
    
    memcpy(config_data_raw.data.get(), &m_config_version, sizeof(m_config_version));
    size_t write_ptr = sizeof(m_config_version);

    for (auto& [ key, value ] : m_cvars) {
        if (value == nullptr)
            continue;

        // key
        memcpy(config_data_raw.data.get() + write_ptr, &key, sizeof(key));
        write_ptr += sizeof(key);

        // m_data_size
        auto size = value->__get_size();
        memcpy(config_data_raw.data.get() + write_ptr, &size, sizeof(size));
        write_ptr += sizeof(size);

        // m_data
        memcpy(config_data_raw.data.get() + write_ptr, value->__get_data(), size);
        write_ptr += size;
    }

    return config_data_raw;
}

bool config::import(const config_data_raw_t& config_data_raw) {
    memcpy(&m_config_version, config_data_raw.data.get(), sizeof(m_config_version));

    if (m_config_version != CREATE_CONFIG_VERSION(1))
        return false;

    size_t read_ptr = sizeof(m_config_version);
    while (read_ptr < config_data_raw.size) {
        uint64_t key = 0;
        memcpy(&key, config_data_raw.data.get() + read_ptr, sizeof(key));
        read_ptr += sizeof(key);

        size_t size = 0;
        memcpy(&size, config_data_raw.data.get() + read_ptr, sizeof(size));
        read_ptr += sizeof(size);

        if (has_cvar(key) && size > 0) {
            auto& _cvar = m_cvars.at(key);
            _cvar->__set_data((uint8_t*)(config_data_raw.data.get() + read_ptr), size);
        }
        
        read_ptr += size;
    }

    return true;
}