#ifndef __CONFIGMANAGER_HPP__
#define __CONFIGMANAGER_HPP__

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../hash.hpp"

#define CONFIG_BASIC "cfg"

template<typename Ty>
struct is_valid_type : std::disjunction<
    std::is_same<Ty, int>,
    std::is_same<Ty, float>,
    std::is_same<Ty, bool>,
    std::is_same<Ty, std::string>,
    std::is_same<Ty, std::vector<int>>,
    std::is_same<Ty, std::vector<float>>
> {};

template<typename Ty>
constexpr bool is_valid_type_v = is_valid_type<Ty>::value;

class icvar {
public:
    virtual char* Data() { return nullptr; }
    virtual void SetData(char*, size_t) {}
    virtual uint64_t Size() { return 0ULL; }
};

template <typename Ty,
    std::enable_if_t<is_valid_type_v<Ty>, bool> = true>
class cvar : public icvar {
public:
    cvar(const Ty& value = Ty{}) : m_value(value) {}

    char* Data() override { return (char*)&m_value; }
    void SetData(char* data, size_t size) override { memcpy(Data(), data, size); };
    uint64_t Size() override { return sizeof(Ty); }

    cvar<Ty>& operator=(const Ty& value) {
        m_value = value;
        return *this;
    }

    cvar<Ty>* operator->() {
        return this;
    }

    operator Ty() {
        return m_value;
    }

    friend std::ostream& operator<<(std::ostream& os, const cvar<Ty>& obj) {
        os << obj.m_value;
        return os;
    }

private:
    Ty m_value;
};

class config {
public:
    struct data_t {
        uint64_t size = 0;
        std::unique_ptr<char[]> data = nullptr;
    };

    config(const std::string& name) : m_name(name) {}

    bool HasVar(const char* key);
    bool HasVar(uint64_t key);

    template <typename Ty>
    config* AddVar(const char* key, Ty data) {
        return AddVar(key, data);
    }

    template <typename Ty>
    config* AddVar(uint64_t key, Ty data) {
        m_data[key] = std::make_unique<cvar<Ty>>(data);
        return this;
    }

    template <typename Ty>
    config* AddTemplate(const char* key) {
        return AddTemplate<Ty>(hash::Fnv1a64HashConst(key));
    }

    template <typename Ty>
    config* AddTemplate(uint64_t key) {
        m_data[key] = std::make_unique<cvar<Ty>>();
        return this;
    }

    template <typename Ty>
    cvar<Ty>& GetVar(const char* key) {
        return GetVar<Ty>(hash::Fnv1a64HashConst(key));
    }

    template <typename Ty>
    cvar<Ty>& GetVar(uint64_t key) {
        return *(static_cast<cvar<Ty>*>(m_data.at(key).get()));
    }

    template <typename Ty>
    config* SetVar(const char* key, Ty value) {
        return SetVar<Ty>(hash::Fnv1a64HashConst(key), value);
    }

    template <typename Ty>
    config* SetVar(uint64_t key, Ty value) {
        GetVar<Ty>(key) = value;
        return this;
    }

    data_t Data();

public:
    std::string m_name;
    std::map<uint64_t, std::unique_ptr<icvar>> m_data = {};
};

class config_manager {
public:
    void Setup();

    void DumpConfig(std::shared_ptr<config> config);
    bool LoadConfig(std::shared_ptr<config> config);

    std::shared_ptr<config> CreateConfig(const std::string& name);
    std::shared_ptr<config> GetConfig(const std::string& name);

private:
    std::map<std::string, std::shared_ptr<config>> m_configs = {};
};

#endif // __CONFIGMANAGER_HPP__