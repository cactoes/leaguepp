#ifndef __DATACHAMPION_HPP__
#define __DATACHAMPION_HPP__

#include "helper.hpp"

namespace data {
    using nlohmann::json;

    #ifndef NLOHMANN_UNTYPED_data_HELPER
    #define NLOHMANN_UNTYPED_data_HELPER
    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }
    #endif

    #ifndef NLOHMANN_OPTIONAL_data_HELPER
    #define NLOHMANN_OPTIONAL_data_HELPER
    template <typename T>
    inline std::shared_ptr<T> get_heap_optional(const json & j, const char * property) {
        auto it = j.find(property);
        if (it != j.end() && !it->is_null()) {
            return j.at(property).get<std::shared_ptr<T>>();
        }
        return std::shared_ptr<T>();
    }

    template <typename T>
    inline std::shared_ptr<T> get_heap_optional(const json & j, std::string property) {
        return get_heap_optional<T>(j, property.data());
    }
    template <typename T>
    inline std::optional<T> get_stack_optional(const json & j, const char * property) {
        auto it = j.find(property);
        if (it != j.end() && !it->is_null()) {
            return j.at(property).get<std::optional<T>>();
        }
        return std::optional<T>();
    }

    template <typename T>
    inline std::optional<T> get_stack_optional(const json & j, std::string property) {
        return get_stack_optional<T>(j, property.data());
    }
    #endif

    enum class Type : int { CHAMPION };

    enum class Sprite : int { CHAMPION0_PNG, CHAMPION1_PNG, CHAMPION2_PNG, CHAMPION3_PNG, CHAMPION4_PNG, CHAMPION5_PNG };

    struct Image {
        std::optional<std::string> full;
        std::optional<Sprite> sprite;
        std::optional<Type> group;
        std::optional<int64_t> x;
        std::optional<int64_t> y;
        std::optional<int64_t> w;
        std::optional<int64_t> h;
    };

    struct Info {
        std::optional<int64_t> attack;
        std::optional<int64_t> defense;
        std::optional<int64_t> magic;
        std::optional<int64_t> difficulty;
    };

    enum class Tag : int { ASSASSIN, FIGHTER, MAGE, MARKSMAN, SUPPORT, TANK };

    struct Datum {
        std::optional<std::string> version;
        std::optional<std::string> id;
        std::optional<std::string> key;
        std::optional<std::string> name;
        std::optional<std::string> title;
        std::optional<std::string> blurb;
        std::optional<Info> info;
        std::optional<Image> image;
        std::optional<std::vector<Tag>> tags;
        std::optional<std::string> partype;
        std::optional<std::map<std::string, double>> stats;
    };

    struct Champion {
        std::optional<Type> type;
        std::optional<std::string> format;
        std::optional<std::string> version;
        std::optional<std::map<std::string, Datum>> data;
    };
}

namespace data {
    void from_json(const json & j, Image & x);
    void to_json(json & j, const Image & x);

    void from_json(const json & j, Info & x);
    void to_json(json & j, const Info & x);

    void from_json(const json & j, Datum & x);
    void to_json(json & j, const Datum & x);

    void from_json(const json & j, Champion & x);
    void to_json(json & j, const Champion & x);

    void from_json(const json & j, Type & x);
    void to_json(json & j, const Type & x);

    void from_json(const json & j, Sprite & x);
    void to_json(json & j, const Sprite & x);

    void from_json(const json & j, Tag & x);
    void to_json(json & j, const Tag & x);

    inline void from_json(const json & j, Image& x) {
        x.full = get_stack_optional<std::string>(j, "full");
        x.sprite = get_stack_optional<Sprite>(j, "sprite");
        x.group = get_stack_optional<Type>(j, "group");
        x.x = get_stack_optional<int64_t>(j, "x");
        x.y = get_stack_optional<int64_t>(j, "y");
        x.w = get_stack_optional<int64_t>(j, "w");
        x.h = get_stack_optional<int64_t>(j, "h");
    }

    inline void to_json(json & j, const Image & x) {
        j = json::object();
        j["full"] = x.full;
        j["sprite"] = x.sprite;
        j["group"] = x.group;
        j["x"] = x.x;
        j["y"] = x.y;
        j["w"] = x.w;
        j["h"] = x.h;
    }

    inline void from_json(const json & j, Info& x) {
        x.attack = get_stack_optional<int64_t>(j, "attack");
        x.defense = get_stack_optional<int64_t>(j, "defense");
        x.magic = get_stack_optional<int64_t>(j, "magic");
        x.difficulty = get_stack_optional<int64_t>(j, "difficulty");
    }

    inline void to_json(json & j, const Info & x) {
        j = json::object();
        j["attack"] = x.attack;
        j["defense"] = x.defense;
        j["magic"] = x.magic;
        j["difficulty"] = x.difficulty;
    }

    inline void from_json(const json & j, Datum& x) {
        x.version = get_stack_optional<std::string>(j, "version");
        x.id = get_stack_optional<std::string>(j, "id");
        x.key = get_stack_optional<std::string>(j, "key");
        x.name = get_stack_optional<std::string>(j, "name");
        x.title = get_stack_optional<std::string>(j, "title");
        x.blurb = get_stack_optional<std::string>(j, "blurb");
        x.info = get_stack_optional<Info>(j, "info");
        x.image = get_stack_optional<Image>(j, "image");
        x.tags = get_stack_optional<std::vector<Tag>>(j, "tags");
        x.partype = get_stack_optional<std::string>(j, "partype");
        x.stats = get_stack_optional<std::map<std::string, double>>(j, "stats");
    }

    inline void to_json(json & j, const Datum & x) {
        j = json::object();
        j["version"] = x.version;
        j["id"] = x.id;
        j["key"] = x.key;
        j["name"] = x.name;
        j["title"] = x.title;
        j["blurb"] = x.blurb;
        j["info"] = x.info;
        j["image"] = x.image;
        j["tags"] = x.tags;
        j["partype"] = x.partype;
        j["stats"] = x.stats;
    }

    inline void from_json(const json & j, Champion& x) {
        x.type = get_stack_optional<Type>(j, "type");
        x.format = get_stack_optional<std::string>(j, "format");
        x.version = get_stack_optional<std::string>(j, "version");
        x.data = get_stack_optional<std::map<std::string, Datum>>(j, "data");
    }

    inline void to_json(json & j, const Champion & x) {
        j = json::object();
        j["type"] = x.type;
        j["format"] = x.format;
        j["version"] = x.version;
        j["data"] = x.data;
    }

    inline void from_json(const json & j, Type & x) {
        if (j == "champion") x = Type::CHAMPION;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Type & x) {
        switch (x) {
            case Type::CHAMPION: j = "champion"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, Sprite & x) {
        if (j == "champion0.png") x = Sprite::CHAMPION0_PNG;
        else if (j == "champion1.png") x = Sprite::CHAMPION1_PNG;
        else if (j == "champion2.png") x = Sprite::CHAMPION2_PNG;
        else if (j == "champion3.png") x = Sprite::CHAMPION3_PNG;
        else if (j == "champion4.png") x = Sprite::CHAMPION4_PNG;
        else if (j == "champion5.png") x = Sprite::CHAMPION5_PNG;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Sprite & x) {
        switch (x) {
            case Sprite::CHAMPION0_PNG: j = "champion0.png"; break;
            case Sprite::CHAMPION1_PNG: j = "champion1.png"; break;
            case Sprite::CHAMPION2_PNG: j = "champion2.png"; break;
            case Sprite::CHAMPION3_PNG: j = "champion3.png"; break;
            case Sprite::CHAMPION4_PNG: j = "champion4.png"; break;
            case Sprite::CHAMPION5_PNG: j = "champion5.png"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, Tag & x) {
        if (j == "Assassin") x = Tag::ASSASSIN;
        else if (j == "Fighter") x = Tag::FIGHTER;
        else if (j == "Mage") x = Tag::MAGE;
        else if (j == "Marksman") x = Tag::MARKSMAN;
        else if (j == "Support") x = Tag::SUPPORT;
        else if (j == "Tank") x = Tag::TANK;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Tag & x) {
        switch (x) {
            case Tag::ASSASSIN: j = "Assassin"; break;
            case Tag::FIGHTER: j = "Fighter"; break;
            case Tag::MAGE: j = "Mage"; break;
            case Tag::MARKSMAN: j = "Marksman"; break;
            case Tag::SUPPORT: j = "Support"; break;
            case Tag::TANK: j = "Tank"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }
}

#endif // __DATACHAMPION_HPP__