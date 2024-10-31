#pragma once

#ifndef __ENDPOINT_MAPPERS_HPP__
#define __ENDPOINT_MAPPERS_HPP__

#include <optional>
#include <stdexcept>
#include <regex>
#include <variant>

#include <nlohmann/json.hpp>

// https://app.quicktype.io/

namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::shared_ptr<T>> {
        static void to_json(json & j, const std::shared_ptr<T> & opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::shared_ptr<T> from_json(const json & j) {
            if (j.is_null()) return std::make_shared<T>(); else return std::make_shared<T>(j.get<T>());
        }
    };
    template <typename T>
    struct adl_serializer<std::optional<T>> {
        static void to_json(json & j, const std::optional<T> & opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::optional<T> from_json(const json & j) {
            if (j.is_null()) return std::make_optional<T>(); else return std::make_optional<T>(j.get<T>());
        }
    };
}

namespace champselect {
    using nlohmann::json;

    #ifndef NLOHMANN_UNTYPED_champselect_HELPER
    #define NLOHMANN_UNTYPED_champselect_HELPER
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

    #ifndef NLOHMANN_OPTIONAL_champselect_HELPER
    #define NLOHMANN_OPTIONAL_champselect_HELPER
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

    struct Action {
        std::optional<int64_t> actorCellId;
        std::optional<int64_t> championId;
        std::optional<bool> completed;
        std::optional<int64_t> id;
        std::optional<bool> isAllyAction;
        std::optional<bool> isInProgress;
        std::optional<int64_t> pickTurn;
        std::optional<std::string> type;
    };

    struct Bans {
        std::optional<std::vector<nlohmann::json>> myTeamBans;
        std::optional<int64_t> numBans;
        std::optional<std::vector<nlohmann::json>> theirTeamBans;
    };

    struct MucJwtDto {
        std::optional<std::string> channelClaim;
        std::optional<std::string> domain;
        std::optional<std::string> jwt;
        std::optional<std::string> targetRegion;
    };

    struct ChatDetails {
        std::optional<MucJwtDto> mucJwtDto;
        std::optional<std::string> multiUserChatId;
        std::optional<std::string> multiUserChatPassword;
    };

    struct MyTeam {
        std::optional<std::string> assignedPosition;
        std::optional<int64_t> cellId;
        std::optional<int64_t> championId;
        std::optional<int64_t> championPickIntent;
        std::optional<std::string> nameVisibilityType;
        std::optional<std::string> obfuscatedPuuid;
        std::optional<int64_t> obfuscatedSummonerId;
        std::optional<std::string> puuid;
        std::optional<int64_t> selectedSkinId;
        std::optional<int64_t> spell1Id;
        std::optional<int64_t> spell2Id;
        std::optional<int64_t> summonerId;
        std::optional<int64_t> team;
        std::optional<int64_t> wardSkinId;
    };

    struct Timer {
        std::optional<int64_t> adjustedTimeLeftInPhase;
        std::optional<int64_t> internalNowInEpochMs;
        std::optional<bool> isInfinite;
        std::optional<std::string> phase;
        std::optional<int64_t> totalTimeInPhase;
    };

    struct Session {
        std::optional<std::vector<std::vector<Action>>> actions;
        std::optional<bool> allowBattleBoost;
        std::optional<bool> allowDuplicatePicks;
        std::optional<bool> allowLockedEvents;
        std::optional<bool> allowRerolling;
        std::optional<bool> allowSkinSelection;
        std::optional<Bans> bans;
        std::optional<std::vector<nlohmann::json>> benchChampions;
        std::optional<bool> benchEnabled;
        std::optional<int64_t> boostableSkinCount;
        std::optional<ChatDetails> chatDetails;
        std::optional<int64_t> counter;
        std::optional<int64_t> gameId;
        std::optional<bool> hasSimultaneousBans;
        std::optional<bool> hasSimultaneousPicks;
        std::optional<bool> isCustomGame;
        std::optional<bool> isSpectating;
        std::optional<int64_t> localPlayerCellId;
        std::optional<int64_t> lockedEventIndex;
        std::optional<std::vector<MyTeam>> myTeam;
        std::optional<std::vector<nlohmann::json>> pickOrderSwaps;
        std::optional<int64_t> recoveryCounter;
        std::optional<int64_t> rerollsRemaining;
        std::optional<bool> skipChampionSelect;
        std::optional<std::vector<nlohmann::json>> theirTeam;
        std::optional<Timer> timer;
        std::optional<std::vector<nlohmann::json>> trades;
    };
}

namespace champselect {
    void from_json(const json & j, Action & x);
    void to_json(json & j, const Action & x);

    void from_json(const json & j, Bans & x);
    void to_json(json & j, const Bans & x);

    void from_json(const json & j, MucJwtDto & x);
    void to_json(json & j, const MucJwtDto & x);

    void from_json(const json & j, ChatDetails & x);
    void to_json(json & j, const ChatDetails & x);

    void from_json(const json & j, MyTeam & x);
    void to_json(json & j, const MyTeam & x);

    void from_json(const json & j, Timer & x);
    void to_json(json & j, const Timer & x);

    void from_json(const json & j, Session & x);
    void to_json(json & j, const Session & x);

    inline void from_json(const json & j, Action& x) {
        x.actorCellId = get_stack_optional<int64_t>(j, "actorCellId");
        x.championId = get_stack_optional<int64_t>(j, "championId");
        x.completed = get_stack_optional<bool>(j, "completed");
        x.id = get_stack_optional<int64_t>(j, "id");
        x.isAllyAction = get_stack_optional<bool>(j, "isAllyAction");
        x.isInProgress = get_stack_optional<bool>(j, "isInProgress");
        x.pickTurn = get_stack_optional<int64_t>(j, "pickTurn");
        x.type = get_stack_optional<std::string>(j, "type");
    }

    inline void to_json(json & j, const Action & x) {
        j = json::object();
        j["actorCellId"] = x.actorCellId;
        j["championId"] = x.championId;
        j["completed"] = x.completed;
        j["id"] = x.id;
        j["isAllyAction"] = x.isAllyAction;
        j["isInProgress"] = x.isInProgress;
        j["pickTurn"] = x.pickTurn;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, Bans& x) {
        x.myTeamBans = get_stack_optional<std::vector<nlohmann::json>>(j, "myTeamBans");
        x.numBans = get_stack_optional<int64_t>(j, "numBans");
        x.theirTeamBans = get_stack_optional<std::vector<nlohmann::json>>(j, "theirTeamBans");
    }

    inline void to_json(json & j, const Bans & x) {
        j = json::object();
        j["myTeamBans"] = x.myTeamBans;
        j["numBans"] = x.numBans;
        j["theirTeamBans"] = x.theirTeamBans;
    }

    inline void from_json(const json & j, MucJwtDto& x) {
        x.channelClaim = get_stack_optional<std::string>(j, "channelClaim");
        x.domain = get_stack_optional<std::string>(j, "domain");
        x.jwt = get_stack_optional<std::string>(j, "jwt");
        x.targetRegion = get_stack_optional<std::string>(j, "targetRegion");
    }

    inline void to_json(json & j, const MucJwtDto & x) {
        j = json::object();
        j["channelClaim"] = x.channelClaim;
        j["domain"] = x.domain;
        j["jwt"] = x.jwt;
        j["targetRegion"] = x.targetRegion;
    }

    inline void from_json(const json & j, ChatDetails& x) {
        x.mucJwtDto = get_stack_optional<MucJwtDto>(j, "mucJwtDto");
        x.multiUserChatId = get_stack_optional<std::string>(j, "multiUserChatId");
        x.multiUserChatPassword = get_stack_optional<std::string>(j, "multiUserChatPassword");
    }

    inline void to_json(json & j, const ChatDetails & x) {
        j = json::object();
        j["mucJwtDto"] = x.mucJwtDto;
        j["multiUserChatId"] = x.multiUserChatId;
        j["multiUserChatPassword"] = x.multiUserChatPassword;
    }

    inline void from_json(const json & j, MyTeam& x) {
        x.assignedPosition = get_stack_optional<std::string>(j, "assignedPosition");
        x.cellId = get_stack_optional<int64_t>(j, "cellId");
        x.championId = get_stack_optional<int64_t>(j, "championId");
        x.championPickIntent = get_stack_optional<int64_t>(j, "championPickIntent");
        x.nameVisibilityType = get_stack_optional<std::string>(j, "nameVisibilityType");
        x.obfuscatedPuuid = get_stack_optional<std::string>(j, "obfuscatedPuuid");
        x.obfuscatedSummonerId = get_stack_optional<int64_t>(j, "obfuscatedSummonerId");
        x.puuid = get_stack_optional<std::string>(j, "puuid");
        x.selectedSkinId = get_stack_optional<int64_t>(j, "selectedSkinId");
        x.spell1Id = get_stack_optional<int64_t>(j, "spell1Id");
        x.spell2Id = get_stack_optional<int64_t>(j, "spell2Id");
        x.summonerId = get_stack_optional<int64_t>(j, "summonerId");
        x.team = get_stack_optional<int64_t>(j, "team");
        x.wardSkinId = get_stack_optional<int64_t>(j, "wardSkinId");
    }

    inline void to_json(json & j, const MyTeam & x) {
        j = json::object();
        j["assignedPosition"] = x.assignedPosition;
        j["cellId"] = x.cellId;
        j["championId"] = x.championId;
        j["championPickIntent"] = x.championPickIntent;
        j["nameVisibilityType"] = x.nameVisibilityType;
        j["obfuscatedPuuid"] = x.obfuscatedPuuid;
        j["obfuscatedSummonerId"] = x.obfuscatedSummonerId;
        j["puuid"] = x.puuid;
        j["selectedSkinId"] = x.selectedSkinId;
        j["spell1Id"] = x.spell1Id;
        j["spell2Id"] = x.spell2Id;
        j["summonerId"] = x.summonerId;
        j["team"] = x.team;
        j["wardSkinId"] = x.wardSkinId;
    }

    inline void from_json(const json & j, Timer& x) {
        x.adjustedTimeLeftInPhase = get_stack_optional<int64_t>(j, "adjustedTimeLeftInPhase");
        x.internalNowInEpochMs = get_stack_optional<int64_t>(j, "internalNowInEpochMs");
        x.isInfinite = get_stack_optional<bool>(j, "isInfinite");
        x.phase = get_stack_optional<std::string>(j, "phase");
        x.totalTimeInPhase = get_stack_optional<int64_t>(j, "totalTimeInPhase");
    }

    inline void to_json(json & j, const Timer & x) {
        j = json::object();
        j["adjustedTimeLeftInPhase"] = x.adjustedTimeLeftInPhase;
        j["internalNowInEpochMs"] = x.internalNowInEpochMs;
        j["isInfinite"] = x.isInfinite;
        j["phase"] = x.phase;
        j["totalTimeInPhase"] = x.totalTimeInPhase;
    }

    inline void from_json(const json & j, Session& x) {
        x.actions = get_stack_optional<std::vector<std::vector<Action>>>(j, "actions");
        x.allowBattleBoost = get_stack_optional<bool>(j, "allowBattleBoost");
        x.allowDuplicatePicks = get_stack_optional<bool>(j, "allowDuplicatePicks");
        x.allowLockedEvents = get_stack_optional<bool>(j, "allowLockedEvents");
        x.allowRerolling = get_stack_optional<bool>(j, "allowRerolling");
        x.allowSkinSelection = get_stack_optional<bool>(j, "allowSkinSelection");
        x.bans = get_stack_optional<Bans>(j, "bans");
        x.benchChampions = get_stack_optional<std::vector<nlohmann::json>>(j, "benchChampions");
        x.benchEnabled = get_stack_optional<bool>(j, "benchEnabled");
        x.boostableSkinCount = get_stack_optional<int64_t>(j, "boostableSkinCount");
        x.chatDetails = get_stack_optional<ChatDetails>(j, "chatDetails");
        x.counter = get_stack_optional<int64_t>(j, "counter");
        x.gameId = get_stack_optional<int64_t>(j, "gameId");
        x.hasSimultaneousBans = get_stack_optional<bool>(j, "hasSimultaneousBans");
        x.hasSimultaneousPicks = get_stack_optional<bool>(j, "hasSimultaneousPicks");
        x.isCustomGame = get_stack_optional<bool>(j, "isCustomGame");
        x.isSpectating = get_stack_optional<bool>(j, "isSpectating");
        x.localPlayerCellId = get_stack_optional<int64_t>(j, "localPlayerCellId");
        x.lockedEventIndex = get_stack_optional<int64_t>(j, "lockedEventIndex");
        x.myTeam = get_stack_optional<std::vector<MyTeam>>(j, "myTeam");
        x.pickOrderSwaps = get_stack_optional<std::vector<nlohmann::json>>(j, "pickOrderSwaps");
        x.recoveryCounter = get_stack_optional<int64_t>(j, "recoveryCounter");
        x.rerollsRemaining = get_stack_optional<int64_t>(j, "rerollsRemaining");
        x.skipChampionSelect = get_stack_optional<bool>(j, "skipChampionSelect");
        x.theirTeam = get_stack_optional<std::vector<nlohmann::json>>(j, "theirTeam");
        x.timer = get_stack_optional<Timer>(j, "timer");
        x.trades = get_stack_optional<std::vector<nlohmann::json>>(j, "trades");
    }

    inline void to_json(json & j, const Session & x) {
        j = json::object();
        j["actions"] = x.actions;
        j["allowBattleBoost"] = x.allowBattleBoost;
        j["allowDuplicatePicks"] = x.allowDuplicatePicks;
        j["allowLockedEvents"] = x.allowLockedEvents;
        j["allowRerolling"] = x.allowRerolling;
        j["allowSkinSelection"] = x.allowSkinSelection;
        j["bans"] = x.bans;
        j["benchChampions"] = x.benchChampions;
        j["benchEnabled"] = x.benchEnabled;
        j["boostableSkinCount"] = x.boostableSkinCount;
        j["chatDetails"] = x.chatDetails;
        j["counter"] = x.counter;
        j["gameId"] = x.gameId;
        j["hasSimultaneousBans"] = x.hasSimultaneousBans;
        j["hasSimultaneousPicks"] = x.hasSimultaneousPicks;
        j["isCustomGame"] = x.isCustomGame;
        j["isSpectating"] = x.isSpectating;
        j["localPlayerCellId"] = x.localPlayerCellId;
        j["lockedEventIndex"] = x.lockedEventIndex;
        j["myTeam"] = x.myTeam;
        j["pickOrderSwaps"] = x.pickOrderSwaps;
        j["recoveryCounter"] = x.recoveryCounter;
        j["rerollsRemaining"] = x.rerollsRemaining;
        j["skipChampionSelect"] = x.skipChampionSelect;
        j["theirTeam"] = x.theirTeam;
        j["timer"] = x.timer;
        j["trades"] = x.trades;
    }
}

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

namespace challenges {
    using nlohmann::json;

    #ifndef NLOHMANN_UNTYPED_challenges_HELPER
    #define NLOHMANN_UNTYPED_challenges_HELPER
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

    #ifndef NLOHMANN_OPTIONAL_challenges_HELPER
    #define NLOHMANN_OPTIONAL_challenges_HELPER
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

    struct CategoryProgress {
        std::optional<std::string> category;
        std::optional<int64_t> current;
        std::optional<std::string> level;
        std::optional<int64_t> max;
        std::optional<double> positionPercentile;
    };

    struct LevelToIconPath {
        std::optional<std::string> bronze;
        std::optional<std::string> challenger;
        std::optional<std::string> diamond;
        std::optional<std::string> gold;
        std::optional<std::string> grandmaster;
        std::optional<std::string> iron;
        std::optional<std::string> master;
        std::optional<std::string> platinum;
        std::optional<std::string> silver;
    };

    struct ChallengeTitleData {
        std::optional<std::string> challengeDescription;
        std::optional<int64_t> challengeId;
        std::optional<std::string> challengeName;
        std::optional<std::string> level;
        std::optional<LevelToIconPath> levelToIconPath;
    };

    struct Title {
        std::optional<std::string> backgroundImagePath;
        std::optional<ChallengeTitleData> challengeTitleData;
        std::optional<std::string> contentId;
        std::optional<std::string> iconPath;
        std::optional<bool> isPermanentTitle;
        std::optional<int64_t> itemId;
        std::optional<std::string> name;
        std::optional<std::string> purchaseDate;
        std::optional<std::string> titleAcquisitionName;
        std::optional<std::string> titleAcquisitionType;
        std::optional<std::string> titleRequirementDescription;
    };

    struct SummaryPlayerData {
        std::optional<int64_t> apexLadderUpdateTime;
        std::optional<int64_t> apexLeaderboardPosition;
        std::optional<std::string> bannerId;
        std::optional<std::vector<CategoryProgress>> categoryProgress;
        std::optional<std::string> crestId;
        std::optional<bool> isApex;
        std::optional<std::string> overallChallengeLevel;
        std::optional<int64_t> pointsUntilNextRank;
        std::optional<double> positionPercentile;
        std::optional<int64_t> prestigeCrestBorderLevel;
        std::optional<std::string> selectedChallengesString;
        std::optional<Title> title;
        std::optional<std::vector<nlohmann::json>> topChallenges;
        std::optional<int64_t> totalChallengeScore;
    };
}

namespace challenges {
    void from_json(const json & j, CategoryProgress & x);
    void to_json(json & j, const CategoryProgress & x);

    void from_json(const json & j, LevelToIconPath & x);
    void to_json(json & j, const LevelToIconPath & x);

    void from_json(const json & j, ChallengeTitleData & x);
    void to_json(json & j, const ChallengeTitleData & x);

    void from_json(const json & j, Title & x);
    void to_json(json & j, const Title & x);

    void from_json(const json & j, SummaryPlayerData & x);
    void to_json(json & j, const SummaryPlayerData & x);

    inline void from_json(const json & j, CategoryProgress& x) {
        x.category = get_stack_optional<std::string>(j, "category");
        x.current = get_stack_optional<int64_t>(j, "current");
        x.level = get_stack_optional<std::string>(j, "level");
        x.max = get_stack_optional<int64_t>(j, "max");
        x.positionPercentile = get_stack_optional<double>(j, "positionPercentile");
    }

    inline void to_json(json & j, const CategoryProgress & x) {
        j = json::object();
        j["category"] = x.category;
        j["current"] = x.current;
        j["level"] = x.level;
        j["max"] = x.max;
        j["positionPercentile"] = x.positionPercentile;
    }

    inline void from_json(const json & j, LevelToIconPath& x) {
        x.bronze = get_stack_optional<std::string>(j, "BRONZE");
        x.challenger = get_stack_optional<std::string>(j, "CHALLENGER");
        x.diamond = get_stack_optional<std::string>(j, "DIAMOND");
        x.gold = get_stack_optional<std::string>(j, "GOLD");
        x.grandmaster = get_stack_optional<std::string>(j, "GRANDMASTER");
        x.iron = get_stack_optional<std::string>(j, "IRON");
        x.master = get_stack_optional<std::string>(j, "MASTER");
        x.platinum = get_stack_optional<std::string>(j, "PLATINUM");
        x.silver = get_stack_optional<std::string>(j, "SILVER");
    }

    inline void to_json(json & j, const LevelToIconPath & x) {
        j = json::object();
        j["BRONZE"] = x.bronze;
        j["CHALLENGER"] = x.challenger;
        j["DIAMOND"] = x.diamond;
        j["GOLD"] = x.gold;
        j["GRANDMASTER"] = x.grandmaster;
        j["IRON"] = x.iron;
        j["MASTER"] = x.master;
        j["PLATINUM"] = x.platinum;
        j["SILVER"] = x.silver;
    }

    inline void from_json(const json & j, ChallengeTitleData& x) {
        x.challengeDescription = get_stack_optional<std::string>(j, "challengeDescription");
        x.challengeId = get_stack_optional<int64_t>(j, "challengeId");
        x.challengeName = get_stack_optional<std::string>(j, "challengeName");
        x.level = get_stack_optional<std::string>(j, "level");
        x.levelToIconPath = get_stack_optional<LevelToIconPath>(j, "levelToIconPath");
    }

    inline void to_json(json & j, const ChallengeTitleData & x) {
        j = json::object();
        j["challengeDescription"] = x.challengeDescription;
        j["challengeId"] = x.challengeId;
        j["challengeName"] = x.challengeName;
        j["level"] = x.level;
        j["levelToIconPath"] = x.levelToIconPath;
    }

    inline void from_json(const json & j, Title& x) {
        x.backgroundImagePath = get_stack_optional<std::string>(j, "backgroundImagePath");
        x.challengeTitleData = get_stack_optional<ChallengeTitleData>(j, "challengeTitleData");
        x.contentId = get_stack_optional<std::string>(j, "contentId");
        x.iconPath = get_stack_optional<std::string>(j, "iconPath");
        x.isPermanentTitle = get_stack_optional<bool>(j, "isPermanentTitle");
        x.itemId = get_stack_optional<int64_t>(j, "itemId");
        x.name = get_stack_optional<std::string>(j, "name");
        x.purchaseDate = get_stack_optional<std::string>(j, "purchaseDate");
        x.titleAcquisitionName = get_stack_optional<std::string>(j, "titleAcquisitionName");
        x.titleAcquisitionType = get_stack_optional<std::string>(j, "titleAcquisitionType");
        x.titleRequirementDescription = get_stack_optional<std::string>(j, "titleRequirementDescription");
    }

    inline void to_json(json & j, const Title & x) {
        j = json::object();
        j["backgroundImagePath"] = x.backgroundImagePath;
        j["challengeTitleData"] = x.challengeTitleData;
        j["contentId"] = x.contentId;
        j["iconPath"] = x.iconPath;
        j["isPermanentTitle"] = x.isPermanentTitle;
        j["itemId"] = x.itemId;
        j["name"] = x.name;
        j["purchaseDate"] = x.purchaseDate;
        j["titleAcquisitionName"] = x.titleAcquisitionName;
        j["titleAcquisitionType"] = x.titleAcquisitionType;
        j["titleRequirementDescription"] = x.titleRequirementDescription;
    }

    inline void from_json(const json & j, SummaryPlayerData& x) {
        x.apexLadderUpdateTime = get_stack_optional<int64_t>(j, "apexLadderUpdateTime");
        x.apexLeaderboardPosition = get_stack_optional<int64_t>(j, "apexLeaderboardPosition");
        x.bannerId = get_stack_optional<std::string>(j, "bannerId");
        x.categoryProgress = get_stack_optional<std::vector<CategoryProgress>>(j, "categoryProgress");
        x.crestId = get_stack_optional<std::string>(j, "crestId");
        x.isApex = get_stack_optional<bool>(j, "isApex");
        x.overallChallengeLevel = get_stack_optional<std::string>(j, "overallChallengeLevel");
        x.pointsUntilNextRank = get_stack_optional<int64_t>(j, "pointsUntilNextRank");
        x.positionPercentile = get_stack_optional<double>(j, "positionPercentile");
        x.prestigeCrestBorderLevel = get_stack_optional<int64_t>(j, "prestigeCrestBorderLevel");
        x.selectedChallengesString = get_stack_optional<std::string>(j, "selectedChallengesString");
        x.title = get_stack_optional<Title>(j, "title");
        x.topChallenges = get_stack_optional<std::vector<nlohmann::json>>(j, "topChallenges");
        x.totalChallengeScore = get_stack_optional<int64_t>(j, "totalChallengeScore");
    }

    inline void to_json(json & j, const SummaryPlayerData & x) {
        j = json::object();
        j["apexLadderUpdateTime"] = x.apexLadderUpdateTime;
        j["apexLeaderboardPosition"] = x.apexLeaderboardPosition;
        j["bannerId"] = x.bannerId;
        j["categoryProgress"] = x.categoryProgress;
        j["crestId"] = x.crestId;
        j["isApex"] = x.isApex;
        j["overallChallengeLevel"] = x.overallChallengeLevel;
        j["pointsUntilNextRank"] = x.pointsUntilNextRank;
        j["positionPercentile"] = x.positionPercentile;
        j["prestigeCrestBorderLevel"] = x.prestigeCrestBorderLevel;
        j["selectedChallengesString"] = x.selectedChallengesString;
        j["title"] = x.title;
        j["topChallenges"] = x.topChallenges;
        j["totalChallengeScore"] = x.totalChallengeScore;
    }
}

namespace lobby {
    using nlohmann::json;

    #ifndef NLOHMANN_UNTYPED_lobby_HELPER
    #define NLOHMANN_UNTYPED_lobby_HELPER
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

    #ifndef NLOHMANN_OPTIONAL_lobby_HELPER
    #define NLOHMANN_OPTIONAL_lobby_HELPER
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

    struct GameConfig {
        std::optional<std::vector<int64_t>> allowablePremadeSizes;
        std::optional<std::string> customLobbyName;
        std::optional<std::string> customMutatorName;
        std::optional<std::vector<nlohmann::json>> customRewardsDisabledReasons;
        std::optional<std::string> customSpectatorPolicy;
        std::optional<std::vector<nlohmann::json>> customSpectators;
        std::optional<std::vector<nlohmann::json>> customTeam100;
        std::optional<std::vector<nlohmann::json>> customTeam200;
        std::optional<std::string> gameMode;
        std::optional<bool> isCustom;
        std::optional<bool> isLobbyFull;
        std::optional<bool> isTeamBuilderManaged;
        std::optional<int64_t> mapId;
        std::optional<int64_t> maxHumanPlayers;
        std::optional<int64_t> maxLobbySize;
        std::optional<int64_t> maxTeamSize;
        std::optional<std::string> pickType;
        std::optional<bool> premadeSizeAllowed;
        std::optional<int64_t> queueId;
        std::optional<bool> shouldForceScarcePositionSelection;
        std::optional<bool> showPositionSelector;
        std::optional<bool> showQuickPlaySlotSelection;
    };

    struct Invitation {
        std::optional<std::string> invitationId;
        std::optional<std::string> invitationType;
        std::optional<std::string> state;
        std::optional<std::string> timestamp;
        std::optional<int64_t> toSummonerId;
        std::optional<std::string> toSummonerName;
    };

    struct Member {
        std::optional<bool> allowedChangeActivity;
        std::optional<bool> allowedInviteOthers;
        std::optional<bool> allowedKickOthers;
        std::optional<bool> allowedStartActivity;
        std::optional<bool> allowedToggleInvite;
        std::optional<bool> autoFillEligible;
        std::optional<bool> autoFillProtectedForPromos;
        std::optional<bool> autoFillProtectedForRemedy;
        std::optional<bool> autoFillProtectedForSoloing;
        std::optional<bool> autoFillProtectedForStreaking;
        std::optional<int64_t> botChampionId;
        std::optional<std::string> botDifficulty;
        std::optional<std::string> botId;
        std::optional<std::string> firstPositionPreference;
        nlohmann::json intraSubteamPosition;
        std::optional<bool> isBot;
        std::optional<bool> isLeader;
        std::optional<bool> isSpectator;
        std::optional<std::vector<nlohmann::json>> playerSlots;
        std::optional<std::string> puuid;
        nlohmann::json quickplayPlayerState;
        std::optional<bool> ready;
        std::optional<std::string> secondPositionPreference;
        std::optional<bool> showGhostedBanner;
        nlohmann::json subteamIndex;
        std::optional<int64_t> summonerIconId;
        std::optional<int64_t> summonerId;
        std::optional<std::string> summonerInternalName;
        std::optional<int64_t> summonerLevel;
        std::optional<std::string> summonerName;
        std::optional<int64_t> teamId;
        std::optional<bool> tftNpeQueueBypass;
    };

    struct MucJwtDto {
        std::optional<std::string> channelClaim;
        std::optional<std::string> domain;
        std::optional<std::string> jwt;
        std::optional<std::string> targetRegion;
    };

    struct Lobby {
        std::optional<bool> canStartActivity;
        std::optional<GameConfig> gameConfig;
        std::optional<std::vector<Invitation>> invitations;
        std::optional<Member> localMember;
        std::optional<std::vector<Member>> members;
        std::optional<MucJwtDto> mucJwtDto;
        std::optional<std::string> multiUserChatId;
        std::optional<std::string> multiUserChatPassword;
        std::optional<std::string> partyId;
        std::optional<std::string> partyType;
        std::optional<std::vector<nlohmann::json>> restrictions;
        std::optional<std::vector<std::string>> scarcePositions;
        std::optional<std::vector<nlohmann::json>> warnings;
    };
}

namespace lobby {
    void from_json(const json & j, GameConfig & x);
    void to_json(json & j, const GameConfig & x);

    void from_json(const json & j, Invitation & x);
    void to_json(json & j, const Invitation & x);

    void from_json(const json & j, Member & x);
    void to_json(json & j, const Member & x);

    void from_json(const json & j, MucJwtDto & x);
    void to_json(json & j, const MucJwtDto & x);

    void from_json(const json & j, Lobby & x);
    void to_json(json & j, const Lobby & x);

    inline void from_json(const json & j, GameConfig& x) {
        x.allowablePremadeSizes = get_stack_optional<std::vector<int64_t>>(j, "allowablePremadeSizes");
        x.customLobbyName = get_stack_optional<std::string>(j, "customLobbyName");
        x.customMutatorName = get_stack_optional<std::string>(j, "customMutatorName");
        x.customRewardsDisabledReasons = get_stack_optional<std::vector<nlohmann::json>>(j, "customRewardsDisabledReasons");
        x.customSpectatorPolicy = get_stack_optional<std::string>(j, "customSpectatorPolicy");
        x.customSpectators = get_stack_optional<std::vector<nlohmann::json>>(j, "customSpectators");
        x.customTeam100 = get_stack_optional<std::vector<nlohmann::json>>(j, "customTeam100");
        x.customTeam200 = get_stack_optional<std::vector<nlohmann::json>>(j, "customTeam200");
        x.gameMode = get_stack_optional<std::string>(j, "gameMode");
        x.isCustom = get_stack_optional<bool>(j, "isCustom");
        x.isLobbyFull = get_stack_optional<bool>(j, "isLobbyFull");
        x.isTeamBuilderManaged = get_stack_optional<bool>(j, "isTeamBuilderManaged");
        x.mapId = get_stack_optional<int64_t>(j, "mapId");
        x.maxHumanPlayers = get_stack_optional<int64_t>(j, "maxHumanPlayers");
        x.maxLobbySize = get_stack_optional<int64_t>(j, "maxLobbySize");
        x.maxTeamSize = get_stack_optional<int64_t>(j, "maxTeamSize");
        x.pickType = get_stack_optional<std::string>(j, "pickType");
        x.premadeSizeAllowed = get_stack_optional<bool>(j, "premadeSizeAllowed");
        x.queueId = get_stack_optional<int64_t>(j, "queueId");
        x.shouldForceScarcePositionSelection = get_stack_optional<bool>(j, "shouldForceScarcePositionSelection");
        x.showPositionSelector = get_stack_optional<bool>(j, "showPositionSelector");
        x.showQuickPlaySlotSelection = get_stack_optional<bool>(j, "showQuickPlaySlotSelection");
    }

    inline void to_json(json & j, const GameConfig & x) {
        j = json::object();
        j["allowablePremadeSizes"] = x.allowablePremadeSizes;
        j["customLobbyName"] = x.customLobbyName;
        j["customMutatorName"] = x.customMutatorName;
        j["customRewardsDisabledReasons"] = x.customRewardsDisabledReasons;
        j["customSpectatorPolicy"] = x.customSpectatorPolicy;
        j["customSpectators"] = x.customSpectators;
        j["customTeam100"] = x.customTeam100;
        j["customTeam200"] = x.customTeam200;
        j["gameMode"] = x.gameMode;
        j["isCustom"] = x.isCustom;
        j["isLobbyFull"] = x.isLobbyFull;
        j["isTeamBuilderManaged"] = x.isTeamBuilderManaged;
        j["mapId"] = x.mapId;
        j["maxHumanPlayers"] = x.maxHumanPlayers;
        j["maxLobbySize"] = x.maxLobbySize;
        j["maxTeamSize"] = x.maxTeamSize;
        j["pickType"] = x.pickType;
        j["premadeSizeAllowed"] = x.premadeSizeAllowed;
        j["queueId"] = x.queueId;
        j["shouldForceScarcePositionSelection"] = x.shouldForceScarcePositionSelection;
        j["showPositionSelector"] = x.showPositionSelector;
        j["showQuickPlaySlotSelection"] = x.showQuickPlaySlotSelection;
    }

    inline void from_json(const json & j, Invitation& x) {
        x.invitationId = get_stack_optional<std::string>(j, "invitationId");
        x.invitationType = get_stack_optional<std::string>(j, "invitationType");
        x.state = get_stack_optional<std::string>(j, "state");
        x.timestamp = get_stack_optional<std::string>(j, "timestamp");
        x.toSummonerId = get_stack_optional<int64_t>(j, "toSummonerId");
        x.toSummonerName = get_stack_optional<std::string>(j, "toSummonerName");
    }

    inline void to_json(json & j, const Invitation & x) {
        j = json::object();
        j["invitationId"] = x.invitationId;
        j["invitationType"] = x.invitationType;
        j["state"] = x.state;
        j["timestamp"] = x.timestamp;
        j["toSummonerId"] = x.toSummonerId;
        j["toSummonerName"] = x.toSummonerName;
    }

    inline void from_json(const json & j, Member& x) {
        x.allowedChangeActivity = get_stack_optional<bool>(j, "allowedChangeActivity");
        x.allowedInviteOthers = get_stack_optional<bool>(j, "allowedInviteOthers");
        x.allowedKickOthers = get_stack_optional<bool>(j, "allowedKickOthers");
        x.allowedStartActivity = get_stack_optional<bool>(j, "allowedStartActivity");
        x.allowedToggleInvite = get_stack_optional<bool>(j, "allowedToggleInvite");
        x.autoFillEligible = get_stack_optional<bool>(j, "autoFillEligible");
        x.autoFillProtectedForPromos = get_stack_optional<bool>(j, "autoFillProtectedForPromos");
        x.autoFillProtectedForRemedy = get_stack_optional<bool>(j, "autoFillProtectedForRemedy");
        x.autoFillProtectedForSoloing = get_stack_optional<bool>(j, "autoFillProtectedForSoloing");
        x.autoFillProtectedForStreaking = get_stack_optional<bool>(j, "autoFillProtectedForStreaking");
        x.botChampionId = get_stack_optional<int64_t>(j, "botChampionId");
        x.botDifficulty = get_stack_optional<std::string>(j, "botDifficulty");
        x.botId = get_stack_optional<std::string>(j, "botId");
        x.firstPositionPreference = get_stack_optional<std::string>(j, "firstPositionPreference");
        x.intraSubteamPosition = get_untyped(j, "intraSubteamPosition");
        x.isBot = get_stack_optional<bool>(j, "isBot");
        x.isLeader = get_stack_optional<bool>(j, "isLeader");
        x.isSpectator = get_stack_optional<bool>(j, "isSpectator");
        x.playerSlots = get_stack_optional<std::vector<nlohmann::json>>(j, "playerSlots");
        x.puuid = get_stack_optional<std::string>(j, "puuid");
        x.quickplayPlayerState = get_untyped(j, "quickplayPlayerState");
        x.ready = get_stack_optional<bool>(j, "ready");
        x.secondPositionPreference = get_stack_optional<std::string>(j, "secondPositionPreference");
        x.showGhostedBanner = get_stack_optional<bool>(j, "showGhostedBanner");
        x.subteamIndex = get_untyped(j, "subteamIndex");
        x.summonerIconId = get_stack_optional<int64_t>(j, "summonerIconId");
        x.summonerId = get_stack_optional<int64_t>(j, "summonerId");
        x.summonerInternalName = get_stack_optional<std::string>(j, "summonerInternalName");
        x.summonerLevel = get_stack_optional<int64_t>(j, "summonerLevel");
        x.summonerName = get_stack_optional<std::string>(j, "summonerName");
        x.teamId = get_stack_optional<int64_t>(j, "teamId");
        x.tftNpeQueueBypass = get_stack_optional<bool>(j, "tftNPEQueueBypass");
    }

    inline void to_json(json & j, const Member & x) {
        j = json::object();
        j["allowedChangeActivity"] = x.allowedChangeActivity;
        j["allowedInviteOthers"] = x.allowedInviteOthers;
        j["allowedKickOthers"] = x.allowedKickOthers;
        j["allowedStartActivity"] = x.allowedStartActivity;
        j["allowedToggleInvite"] = x.allowedToggleInvite;
        j["autoFillEligible"] = x.autoFillEligible;
        j["autoFillProtectedForPromos"] = x.autoFillProtectedForPromos;
        j["autoFillProtectedForRemedy"] = x.autoFillProtectedForRemedy;
        j["autoFillProtectedForSoloing"] = x.autoFillProtectedForSoloing;
        j["autoFillProtectedForStreaking"] = x.autoFillProtectedForStreaking;
        j["botChampionId"] = x.botChampionId;
        j["botDifficulty"] = x.botDifficulty;
        j["botId"] = x.botId;
        j["firstPositionPreference"] = x.firstPositionPreference;
        j["intraSubteamPosition"] = x.intraSubteamPosition;
        j["isBot"] = x.isBot;
        j["isLeader"] = x.isLeader;
        j["isSpectator"] = x.isSpectator;
        j["playerSlots"] = x.playerSlots;
        j["puuid"] = x.puuid;
        j["quickplayPlayerState"] = x.quickplayPlayerState;
        j["ready"] = x.ready;
        j["secondPositionPreference"] = x.secondPositionPreference;
        j["showGhostedBanner"] = x.showGhostedBanner;
        j["subteamIndex"] = x.subteamIndex;
        j["summonerIconId"] = x.summonerIconId;
        j["summonerId"] = x.summonerId;
        j["summonerInternalName"] = x.summonerInternalName;
        j["summonerLevel"] = x.summonerLevel;
        j["summonerName"] = x.summonerName;
        j["teamId"] = x.teamId;
        j["tftNPEQueueBypass"] = x.tftNpeQueueBypass;
    }

    inline void from_json(const json & j, MucJwtDto& x) {
        x.channelClaim = get_stack_optional<std::string>(j, "channelClaim");
        x.domain = get_stack_optional<std::string>(j, "domain");
        x.jwt = get_stack_optional<std::string>(j, "jwt");
        x.targetRegion = get_stack_optional<std::string>(j, "targetRegion");
    }

    inline void to_json(json & j, const MucJwtDto & x) {
        j = json::object();
        j["channelClaim"] = x.channelClaim;
        j["domain"] = x.domain;
        j["jwt"] = x.jwt;
        j["targetRegion"] = x.targetRegion;
    }

    inline void from_json(const json & j, Lobby& x) {
        x.canStartActivity = get_stack_optional<bool>(j, "canStartActivity");
        x.gameConfig = get_stack_optional<GameConfig>(j, "gameConfig");
        x.invitations = get_stack_optional<std::vector<Invitation>>(j, "invitations");
        x.localMember = get_stack_optional<Member>(j, "localMember");
        x.members = get_stack_optional<std::vector<Member>>(j, "members");
        x.mucJwtDto = get_stack_optional<MucJwtDto>(j, "mucJwtDto");
        x.multiUserChatId = get_stack_optional<std::string>(j, "multiUserChatId");
        x.multiUserChatPassword = get_stack_optional<std::string>(j, "multiUserChatPassword");
        x.partyId = get_stack_optional<std::string>(j, "partyId");
        x.partyType = get_stack_optional<std::string>(j, "partyType");
        x.restrictions = get_stack_optional<std::vector<nlohmann::json>>(j, "restrictions");
        x.scarcePositions = get_stack_optional<std::vector<std::string>>(j, "scarcePositions");
        x.warnings = get_stack_optional<std::vector<nlohmann::json>>(j, "warnings");
    }

    inline void to_json(json & j, const Lobby & x) {
        j = json::object();
        j["canStartActivity"] = x.canStartActivity;
        j["gameConfig"] = x.gameConfig;
        j["invitations"] = x.invitations;
        j["localMember"] = x.localMember;
        j["members"] = x.members;
        j["mucJwtDto"] = x.mucJwtDto;
        j["multiUserChatId"] = x.multiUserChatId;
        j["multiUserChatPassword"] = x.multiUserChatPassword;
        j["partyId"] = x.partyId;
        j["partyType"] = x.partyType;
        j["restrictions"] = x.restrictions;
        j["scarcePositions"] = x.scarcePositions;
        j["warnings"] = x.warnings;
    }
}

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

namespace datalocal {
    using nlohmann::json;

    #ifndef NLOHMANN_UNTYPED_datalocal_HELPER
    #define NLOHMANN_UNTYPED_datalocal_HELPER
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

    #ifndef NLOHMANN_OPTIONAL_datalocal_HELPER
    #define NLOHMANN_OPTIONAL_datalocal_HELPER
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

    using Datum = std::variant<int64_t, std::string>;

    struct Resource {
        std::string version;
        std::vector<std::array<Datum, 2>> data;
    };
}

namespace datalocal {
void from_json(const json & j, Resource & x);
void to_json(json & j, const Resource & x);
}
namespace nlohmann {
template <>
struct adl_serializer<std::variant<int64_t, std::string>> {
    static void from_json(const json & j, std::variant<int64_t, std::string> & x);
    static void to_json(json & j, const std::variant<int64_t, std::string> & x);
};
}
namespace datalocal {
    inline void from_json(const json & j, Resource& x) {
        x.version = j.at("version").get<std::string>();
        x.data = j.at("data").get<std::vector<std::array<Datum, 2>>>();
    }

    inline void to_json(json & j, const Resource & x) {
        j = json::object();
        j["version"] = x.version;
        j["data"] = x.data;
    }
}
namespace nlohmann {
    inline void adl_serializer<std::variant<int64_t, std::string>>::from_json(const json & j, std::variant<int64_t, std::string> & x) {
        if (j.is_number_integer())
            x = j.get<int64_t>();
        else if (j.is_string())
            x = j.get<std::string>();
        else throw std::runtime_error("Could not deserialise!");
    }

    inline void adl_serializer<std::variant<int64_t, std::string>>::to_json(json & j, const std::variant<int64_t, std::string> & x) {
        switch (x.index()) {
            case 0:
                j = std::get<int64_t>(x);
                break;
            case 1:
                j = std::get<std::string>(x);
                break;
            default: throw std::runtime_error("Input JSON does not conform to schema!");
        }
    }
}

#endif // __ENDPOINT_MAPPERS_HPP__