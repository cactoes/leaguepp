#pragma once

#ifndef __CONFIG_MANAGER_HPP__
#define __CONFIG_MANAGER_HPP__

#include <map>
#include <fstream>
#include <optional>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

#include <reflection/utils.hpp>

#include "manager.hpp"

#define USER_SETTINGS_CONFIG "user-settings"
#define CREATE_CONFIG_VERSION(v) (uint64_t)(v)
#define ACTIVE_CONFIG_VERSION CREATE_CONFIG_VERSION(1)

class abstract_cvar {
public:
    virtual ~abstract_cvar() = default;

    virtual uint8_t* __get_data() = 0;
    virtual void __set_data(uint8_t* data, size_t size) = 0;
    virtual size_t __get_size() const = 0;
};

template <typename _data_type>
class cvar : public abstract_cvar {
public:
    cvar(const _data_type& value)
        : m_data(value) {};

    uint8_t* __get_data() override {
        return (uint8_t*)&m_data;
    }

    void __set_data(uint8_t* data, size_t size) override {
        memcpy(__get_data(), data, size);
    }

    size_t __get_size() const override {
        return sizeof(_data_type);
    }

    _data_type& get() {
        return m_data;
    }

private:
    _data_type m_data;
};

template <typename _data_type>
class cvar<std::vector<_data_type>> : public abstract_cvar {
public:
    typedef std::vector<_data_type> _vector_type;

    cvar(const _vector_type& value)
        : m_data(value) {};

    uint8_t* __get_data() override {
        return (uint8_t*)m_data.data();
    }

    void __set_data(uint8_t* data, size_t size) override {
        m_data.resize(size / sizeof(_data_type));
        memcpy(m_data.data(), data, size);
    }

    size_t __get_size() const override {
        return sizeof(_data_type) * m_data.size();
    }

    _vector_type& get() {
        return m_data;
    }

private:
    _vector_type m_data;
};

template <>
class cvar<std::string> : public abstract_cvar {
public:
    typedef std::string _str_type;

    cvar(const _str_type& value)
        : m_data(value) {};

    uint8_t* __get_data() override {
        return (uint8_t*)&m_data[0];
    }

    void __set_data(uint8_t* data, size_t size) override {
        m_data.resize(size);
        memcpy(&m_data[0], data, size);
    }

    size_t __get_size() const override {
        return m_data.size();
    }

    _str_type& get() {
        return m_data;
    }

private:
    _str_type m_data;
};

class config {
public:
    config(const std::string& name, uint64_t config_version = ACTIVE_CONFIG_VERSION)
        : m_name(name), m_config_version(config_version) {}

    template <typename Ty>
    void add_cvar(const char* key, Ty data) {
        const auto key_hash = ru_hash_fnv1a_64(key);
        m_cvars[key_hash] = std::make_unique<cvar<Ty>>(data);
    }

    template <typename Ty>
    void add_template(const char* key) {
        add_cvar(key, Ty{});
    }

    template <typename Ty>
    void add_template(const std::string& key) {
        add_cvar(key.c_str(), Ty{});
    }

    bool has_cvar(uint64_t key) {
        return m_cvars.contains(key);
    }

    template <typename Ty>
    std::optional<cvar<Ty>*> get_cvar(const char* key) {
        const auto key_hash = ru_hash_fnv1a_64(key);
        if (!m_cvars.contains(key_hash))
            return std::nullopt;
        
        return static_cast<cvar<Ty>*>(m_cvars.at(key_hash).get());
    }

    template <typename Ty>
    cvar<Ty>* get_cvar_u(const char* key) {
        return get_cvar<Ty>(key).value();
    }

    template <typename Ty>
    cvar<Ty>* get_cvar_u(const std::string& key) {
        return get_cvar<Ty>(key.c_str()).value();
    }

    template <typename Ty>
    bool set_cvar(const char* key, Ty data) {
        std::optional<cvar<Ty>*> cvar = get_cvar<Ty>(key);
        if (!cvar.has_value())
            return false;

        cvar.value()->get() = data;
        return true;
    }

    struct config_data_raw_t {
        std::unique_ptr<uint8_t[]> data;
        size_t size;
    };

    config_data_raw_t dump() const;
    bool import(const config_data_raw_t& config_data_raw);

public:
    std::string m_name;

private:
    uint64_t m_config_version;
    std::map<uint64_t, std::unique_ptr<abstract_cvar>> m_cvars {};
};

typedef std::shared_ptr<config> config_handle;

class config_manager : manager::abstract {
public:
    bool setup() override { return true; }
    bool shutdown() override { return true; }

    bool load_config(config_handle config);
    bool dump_config(config_handle config);

    config_handle get_config(const std::string& name);
    bool save_config(config_handle config);
    config_handle create_config(const std::string& name);

    template <typename Ty>
    bool tracked_set_cvar(config_handle config, const char* key, Ty data) {
        config->set_cvar<Ty>(key, data);
        return dump_config(config);
    }

private:
    std::map<std::string, config_handle> m_configs {};
};

#endif // __CONFIG_MANAGER_HPP__