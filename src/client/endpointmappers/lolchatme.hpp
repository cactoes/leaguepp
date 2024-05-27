#ifndef __LOLCHATME_HPP__
#define __LOLCHATME_HPP__

#include "helper.hpp"

namespace lolchat {
    using nlohmann::json;

    #ifndef NLOHMANN_UNTYPED_lolchat_HELPER
    #define NLOHMANN_UNTYPED_lolchat_HELPER
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

    #ifndef NLOHMANN_OPTIONAL_lolchat_HELPER
    #define NLOHMANN_OPTIONAL_lolchat_HELPER
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

    struct Lol {
        std::optional<std::string> bannerIdSelected;
        std::optional<std::string> challengeCrystalLevel;
        std::optional<std::string> challengePoints;
        std::optional<std::string> challengeTitleSelected;
        std::optional<std::string> challengeTokensSelected;
        std::optional<std::string> championId;
        std::optional<std::string> companionId;
        std::optional<std::string> damageSkinId;
        std::optional<std::string> gameQueueType;
        std::optional<std::string> gameStatus;
        std::optional<std::string> iconOverride;
        std::optional<std::string> level;
        std::optional<std::string> mapId;
        std::optional<std::string> mapSkinId;
        std::optional<std::string> legendaryMasteryScore;
        std::optional<std::string> puuid;
        std::optional<std::string> rankedLeagueDivision;
        std::optional<std::string> rankedLeagueQueue;
        std::optional<std::string> rankedLeagueTier;
        std::optional<std::string> rankedLosses;
        std::optional<std::string> rankedPrevSeasonDivision;
        std::optional<std::string> rankedPrevSeasonTier;
        std::optional<std::string> rankedSplitRewardLevel;
        std::optional<std::string> rankedWins;
        std::optional<std::string> regalia;
        std::optional<std::string> skinVariant;
        std::optional<std::string> skinname;
    };

    struct Me {
        std::optional<std::string> availability;
        std::optional<std::string> gameName;
        std::optional<std::string> gameTag;
        std::optional<int64_t> icon;
        std::optional<std::string> id;
        nlohmann::json lastSeenOnlineTimestamp;
        std::optional<Lol> lol;
        std::optional<std::string> name;
        std::optional<int64_t> obfuscatedSummonerId;
        std::optional<std::string> patchline;
        std::optional<std::string> pid;
        std::optional<std::string> platformId;
        std::optional<std::string> product;
        std::optional<std::string> productName;
        std::optional<std::string> puuid;
        std::optional<std::string> statusMessage;
        std::optional<std::string> summary;
        std::optional<int64_t> summonerId;
        std::optional<int64_t> time;
    };
}

namespace lolchat {
    void from_json(const json & j, Lol & x);
    void to_json(json & j, const Lol & x);

    void from_json(const json & j, Me & x);
    void to_json(json & j, const Me & x);

    inline void from_json(const json & j, Lol& x) {
        x.bannerIdSelected = get_stack_optional<std::string>(j, "bannerIdSelected");
        x.challengeCrystalLevel = get_stack_optional<std::string>(j, "challengeCrystalLevel");
        x.challengePoints = get_stack_optional<std::string>(j, "challengePoints");
        x.challengeTitleSelected = get_stack_optional<std::string>(j, "challengeTitleSelected");
        x.challengeTokensSelected = get_stack_optional<std::string>(j, "challengeTokensSelected");
        x.championId = get_stack_optional<std::string>(j, "championId");
        x.companionId = get_stack_optional<std::string>(j, "companionId");
        x.damageSkinId = get_stack_optional<std::string>(j, "damageSkinId");
        x.gameQueueType = get_stack_optional<std::string>(j, "gameQueueType");
        x.gameStatus = get_stack_optional<std::string>(j, "gameStatus");
        x.iconOverride = get_stack_optional<std::string>(j, "iconOverride");
        x.level = get_stack_optional<std::string>(j, "level");
        x.mapId = get_stack_optional<std::string>(j, "mapId");
        x.mapSkinId = get_stack_optional<std::string>(j, "mapSkinId");
        x.legendaryMasteryScore = get_stack_optional<std::string>(j, "legendaryMasteryScore");
        x.puuid = get_stack_optional<std::string>(j, "puuid");
        x.rankedLeagueDivision = get_stack_optional<std::string>(j, "rankedLeagueDivision");
        x.rankedLeagueQueue = get_stack_optional<std::string>(j, "rankedLeagueQueue");
        x.rankedLeagueTier = get_stack_optional<std::string>(j, "rankedLeagueTier");
        x.rankedLosses = get_stack_optional<std::string>(j, "rankedLosses");
        x.rankedPrevSeasonDivision = get_stack_optional<std::string>(j, "rankedPrevSeasonDivision");
        x.rankedPrevSeasonTier = get_stack_optional<std::string>(j, "rankedPrevSeasonTier");
        x.rankedSplitRewardLevel = get_stack_optional<std::string>(j, "rankedSplitRewardLevel");
        x.rankedWins = get_stack_optional<std::string>(j, "rankedWins");
        x.regalia = get_stack_optional<std::string>(j, "regalia");
        x.skinVariant = get_stack_optional<std::string>(j, "skinVariant");
        x.skinname = get_stack_optional<std::string>(j, "skinname");
    }

    inline void to_json(json & j, const Lol & x) {
        j = json::object();
        j["bannerIdSelected"] = x.bannerIdSelected;
        j["challengeCrystalLevel"] = x.challengeCrystalLevel;
        j["challengePoints"] = x.challengePoints;
        j["challengeTitleSelected"] = x.challengeTitleSelected;
        j["challengeTokensSelected"] = x.challengeTokensSelected;
        j["championId"] = x.championId;
        j["companionId"] = x.companionId;
        j["damageSkinId"] = x.damageSkinId;
        j["gameQueueType"] = x.gameQueueType;
        j["gameStatus"] = x.gameStatus;
        j["iconOverride"] = x.iconOverride;
        j["level"] = x.level;
        j["mapId"] = x.mapId;
        j["mapSkinId"] = x.mapSkinId;
        j["legendaryMasteryScore"] = x.legendaryMasteryScore;
        j["puuid"] = x.puuid;
        j["rankedLeagueDivision"] = x.rankedLeagueDivision;
        j["rankedLeagueQueue"] = x.rankedLeagueQueue;
        j["rankedLeagueTier"] = x.rankedLeagueTier;
        j["rankedLosses"] = x.rankedLosses;
        j["rankedPrevSeasonDivision"] = x.rankedPrevSeasonDivision;
        j["rankedPrevSeasonTier"] = x.rankedPrevSeasonTier;
        j["rankedSplitRewardLevel"] = x.rankedSplitRewardLevel;
        j["rankedWins"] = x.rankedWins;
        j["regalia"] = x.regalia;
        j["skinVariant"] = x.skinVariant;
        j["skinname"] = x.skinname;
    }

    inline void from_json(const json & j, Me& x) {
        x.availability = get_stack_optional<std::string>(j, "availability");
        x.gameName = get_stack_optional<std::string>(j, "gameName");
        x.gameTag = get_stack_optional<std::string>(j, "gameTag");
        x.icon = get_stack_optional<int64_t>(j, "icon");
        x.id = get_stack_optional<std::string>(j, "id");
        x.lastSeenOnlineTimestamp = get_untyped(j, "lastSeenOnlineTimestamp");
        x.lol = get_stack_optional<Lol>(j, "lol");
        x.name = get_stack_optional<std::string>(j, "name");
        x.obfuscatedSummonerId = get_stack_optional<int64_t>(j, "obfuscatedSummonerId");
        x.patchline = get_stack_optional<std::string>(j, "patchline");
        x.pid = get_stack_optional<std::string>(j, "pid");
        x.platformId = get_stack_optional<std::string>(j, "platformId");
        x.product = get_stack_optional<std::string>(j, "product");
        x.productName = get_stack_optional<std::string>(j, "productName");
        x.puuid = get_stack_optional<std::string>(j, "puuid");
        x.statusMessage = get_stack_optional<std::string>(j, "statusMessage");
        x.summary = get_stack_optional<std::string>(j, "summary");
        x.summonerId = get_stack_optional<int64_t>(j, "summonerId");
        x.time = get_stack_optional<int64_t>(j, "time");
    }

    inline void to_json(json & j, const Me & x) {
        j = json::object();
        j["availability"] = x.availability;
        j["gameName"] = x.gameName;
        j["gameTag"] = x.gameTag;
        j["icon"] = x.icon;
        j["id"] = x.id;
        j["lastSeenOnlineTimestamp"] = x.lastSeenOnlineTimestamp;
        j["lol"] = x.lol;
        j["name"] = x.name;
        j["obfuscatedSummonerId"] = x.obfuscatedSummonerId;
        j["patchline"] = x.patchline;
        j["pid"] = x.pid;
        j["platformId"] = x.platformId;
        j["product"] = x.product;
        j["productName"] = x.productName;
        j["puuid"] = x.puuid;
        j["statusMessage"] = x.statusMessage;
        j["summary"] = x.summary;
        j["summonerId"] = x.summonerId;
        j["time"] = x.time;
    }
}

#endif // __LOLCHATME_HPP__