#ifndef __CONFIGMANAGER_HPP__
#define __CONFIGMANAGER_HPP__

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <hash.hpp>

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

class ICvar {
public:
    virtual ~ICvar() = default;

    virtual char* Data() { return nullptr; }
    virtual void SetData(char*, size_t) {}
    virtual uint64_t Size() { return 0ULL; }
};

template <typename Ty,
    std::enable_if_t<is_valid_type_v<Ty>, bool> = true>
class Cvar : public ICvar {
public:
    Cvar(const Ty& value = Ty{}) : m_value(value) {}

    char* Data() override { return (char*)&m_value; }
    void SetData(char* data, size_t size) override { memcpy(Data(), data, size); };
    uint64_t Size() override { return sizeof(Ty); }

    Cvar<Ty>& operator=(const Ty& value) {
        m_value = value;
        return *this;
    }

    Cvar<Ty>* operator->() {
        return this;
    }

    operator Ty() {
        return m_value;
    }

    friend std::ostream& operator<<(std::ostream& os, const Cvar<Ty>& obj) {
        os << obj.m_value;
        return os;
    }

private:
    Ty m_value;
};

class Config {
public:
    struct data_t {
        uint64_t size = 0;
        std::unique_ptr<char[]> data = nullptr;
    };

    Config(const std::string& name) : m_name(name) {}

    bool HasVar(const char* key);
    bool HasVar(uint64_t key);

    template <typename Ty>
    Config* AddVar(const char* key, Ty data) {
        return AddVar(key, data);
    }

    template <typename Ty>
    Config* AddVar(uint64_t key, Ty data) {
        m_data[key] = std::make_unique<Cvar<Ty>>(data);
        return this;
    }

    template <typename Ty>
    Config* AddTemplate(const char* key) {
        return AddTemplate<Ty>(hash::Fnv1a64HashConst(key));
    }

    template <typename Ty>
    Config* AddTemplate(uint64_t key) {
        m_data[key] = std::make_unique<Cvar<Ty>>();
        return this;
    }

    template <typename Ty>
    Cvar<Ty>& GetVar(const char* key) {
        return GetVar<Ty>(hash::Fnv1a64HashConst(key));
    }

    template <typename Ty>
    Cvar<Ty>& GetVar(uint64_t key) {
        return *(static_cast<Cvar<Ty>*>(m_data.at(key).get()));
    }

    template <typename Ty>
    Config* SetVar(const char* key, Ty value) {
        return SetVar<Ty>(hash::Fnv1a64HashConst(key), value);
    }

    template <typename Ty>
    Config* SetVar(uint64_t key, Ty value) {
        GetVar<Ty>(key) = value;
        return this;
    }

    data_t Data();

public:
    std::string m_name;
    std::map<uint64_t, std::unique_ptr<ICvar>> m_data = {};
};

typedef std::shared_ptr<Config> config_handle;

class ConfigManager {
public:
    bool Init();

    bool Dump(config_handle config);
    bool Load(config_handle config);

    config_handle Create(const std::string& name);
    config_handle Get(const std::string& name);

    template <typename Ty>
    Ty TrackedSetVar(config_handle config, const char* key, Ty value) {
        config->SetVar(key, value);
        Dump(config);
        return value;
    }

private:
    std::map<std::string, config_handle> m_configs = {};
};

#endif // __CONFIGMANAGER_HPP__