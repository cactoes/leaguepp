#ifndef __CHAMPSELECTSESSION_HPP__
#define __CHAMPSELECTSESSION_HPP__

#include "helper.hpp"

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

#endif // __CHAMPSELECTSESSION_HPP__