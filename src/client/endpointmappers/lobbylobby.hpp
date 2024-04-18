#ifndef __LOBBYLOBBY_HPP__
#define __LOBBYLOBBY_HPP__

#include "helper.hpp"

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

#endif // __LOBBYLOBBY_HPP__