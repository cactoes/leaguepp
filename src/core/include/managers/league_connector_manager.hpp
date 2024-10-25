#pragma once

#ifndef __LEAGUE_CONNECTOR_MANAGER_HPP__
#define __LEAGUE_CONNECTOR_MANAGER_HPP__

#include <connector.hpp>
#include <vui/utils.hpp>

#include "../endpoint_mappers.hpp"
#include "manager.hpp"
#include "../utils.hpp"

namespace league_constants {
    struct kv_t {
        std::string name;
        uint64_t hash;
        size_t index;
    };

    inline uint64_t hash_constant(const std::string& value) {
        return vu_hash_string_64(value.c_str());
    }

    inline constexpr uint64_t hash_constant(const char* value) {
        return vu_hash_string_64(value);
    }

    template <size_t _array_size>
    inline std::optional<size_t> get_index_of(const std::string& target, const std::array<kv_t, _array_size>& lookup_array) {
        if (auto it = std::ranges::find_if(lookup_array, [&target](const kv_t& value) { return value.name == target; }); it != lookup_array.end())
            return static_cast<size_t>(std::distance(lookup_array.begin(), it));

        return std::nullopt;
    }

    template <size_t _array_size>
    inline std::optional<size_t> get_index_of_hashed(uint64_t target, const std::array<kv_t, _array_size>& lookup_array) {
        if (auto it = std::ranges::find(lookup_array, [&target](const kv_t& value) { return value.hash == target; }); it != lookup_array.end())
            return static_cast<size_t>(std::distance(lookup_array.begin(), it));

        return std::nullopt;
    }

    template <typename _struct_type, size_t _array_size, typename _match_func>
        requires std::is_invocable_r_v<bool, _match_func, const _struct_type&>
    inline std::optional<std::reference_wrapper<const _struct_type>> get_by(const std::array<_struct_type, _array_size>& array, _match_func&& matcher) {
        if (auto it = std::ranges::find_if(array, matcher); it != array.end())
            return std::cref(*it);

        return std::nullopt;
    }

    template <size_t _array_size>
    inline std::optional<std::reference_wrapper<const kv_t>> get_struct(uint64_t hash, const std::array<kv_t, _array_size>& lookup_array) {
        return get_by(lookup_array, [&hash](const auto& v) { return v.hash == hash; });
    }

    template <size_t _array_size>
    inline std::optional<std::reference_wrapper<const kv_t>>  get_struct(const std::string& name, const std::array<kv_t, _array_size>& lookup_array) {
        return get_by(lookup_array, [&name](const auto& v) { return v.name == name; });
    }

    template <size_t _array_size>
    inline std::vector<std::string> get_names(const std::array<kv_t, _array_size>& lookup_array) {
        std::vector<std::string> out_array {};
        out_array.reserve(_array_size);

        for (const auto& v : lookup_array)
            out_array.emplace_back(v.name);

        return out_array;
    }

    namespace lanes {
        enum array_index_t {
            AI_TOP = 0,
            AI_JUNGLE,
            AI_MIDDLE,
            AI_BOTTOM,
            AI_UTILITY,
        };

        static constexpr uint64_t top = hash_constant("top");
        static constexpr uint64_t jungle = hash_constant("jungle");
        static constexpr uint64_t middle = hash_constant("middle");
        static constexpr uint64_t bottom = hash_constant("bottom");
        static constexpr uint64_t utility = hash_constant("utility");

        static const std::array<kv_t, 5> list = {
            kv_t{ "top", top, 0 },
            kv_t{ "jungle", jungle, 1 },
            kv_t{ "middle", middle, 2 },
            kv_t{ "bottom", bottom, 3 },
            kv_t{ "utility", utility, 4 }
        };
    } // namespace lanes

    namespace party_type {
        enum array_index_t {
            AI_OPEN = 0,
            AI_CLOSED,
        };

        static constexpr uint64_t open = hash_constant("open");
        static constexpr uint64_t closed = hash_constant("closed");

        static const std::array<kv_t, 2> list = {
            kv_t{ "open", open },
            kv_t{ "closed", closed }
        };
    } // namespace party_type

    namespace division {
        enum array_index_t {
            AI_I = 0,
            AI_II,
            AI_III,
            AI_IV,
        };

        static constexpr uint64_t I = hash_constant("I");
        static constexpr uint64_t II = hash_constant("II");
        static constexpr uint64_t III = hash_constant("III");
        static constexpr uint64_t IV = hash_constant("IV");

        static const std::array<kv_t, 4> list = {
            kv_t{ "I", I, 0 },
            kv_t{ "II", II, 1 },
            kv_t{ "III", III, 2 },
            kv_t{ "IV", IV, 3 }
        };
    } // namespace division

    namespace tier {
        enum array_index_t {
            AI_IRON = 0,
            AI_BRONZE,
            AI_SILVER,
            AI_GOLD,
            AI_PLATINUM,
            AI_EMERALD,
            AI_DIAMOND,
            AI_MASTER,
            AI_GRANDMASTER,
            AI_CHALLENGER
        };

        static constexpr uint64_t iron = hash_constant("iron");
        static constexpr uint64_t bronze = hash_constant("bronze");
        static constexpr uint64_t silver = hash_constant("silver");
        static constexpr uint64_t gold = hash_constant("gold");
        static constexpr uint64_t platinum = hash_constant("platinum");
        static constexpr uint64_t emerald = hash_constant("emerald");
        static constexpr uint64_t diamond = hash_constant("diamond");
        static constexpr uint64_t master = hash_constant("master");
        static constexpr uint64_t grandmaster = hash_constant("grandmaster");
        static constexpr uint64_t challenger = hash_constant("challenger");

        static const std::array<kv_t, 10> list = {
            kv_t{ "iron", iron, 0 },
            kv_t{ "bronze", bronze, 1 },
            kv_t{ "silver", silver , 2},
            kv_t{ "gold", gold, 3 },
            kv_t{ "platinum", platinum, 4 },
            kv_t{ "emerald", emerald, 5 },
            kv_t{ "diamond", diamond, 6 },
            kv_t{ "master", master, 7 },
            kv_t{ "grandmaster", grandmaster, 8 },
            kv_t{ "challenger", challenger, 9  }
        };
    } // namespace tier

    namespace gameflow {
        enum array_index_t {
            AI_NONE = 0,
            AI_LOBBY,
            AI_MATCHMAKING,
            AI_READYCHECK,
            AI_CHAMPSELECT,
            AI_INPROGRESS,
            AI_WAITING_FOR_STATS,
            AI_END_OF_GAME,
        };

        static constexpr uint64_t none = hash_constant("None");
        static constexpr uint64_t lobby = hash_constant("Lobby");
        static constexpr uint64_t matchmaking = hash_constant("Matchmaking");
        static constexpr uint64_t readycheck = hash_constant("ReadyCheck");
        static constexpr uint64_t champselect = hash_constant("ChampSelect");
        static constexpr uint64_t in_progress = hash_constant("InProgress");
        static constexpr uint64_t waiting_for_stats = hash_constant("WaitingForStats");
        static constexpr uint64_t end_of_game = hash_constant("EndOfGame");

        static const std::array<kv_t, 8> list = {
            kv_t{ "None", none, 0 },
            kv_t{ "Lobby", lobby, 1 },
            kv_t{ "Matchmaking", matchmaking, 2 },
            kv_t{ "ReadyCheck", readycheck, 3 },
            kv_t{ "ChampSelect", champselect, 4 },
            kv_t{ "InProgress", in_progress, 5 },
            kv_t{ "WaitingForStats", waiting_for_stats, 6 },
            kv_t{ "EndOfGame", end_of_game, 7 }
        };
    } // namespace gameflow

    namespace queue {
        struct queue_t {
            uint64_t id;
            std::string name;
        };

        enum queue_array_index_t {
            NORMAL_DRAFT = 0,
            RANKED_SOLO_5x5,
            NORMAL_BLIND,
            RANKED_FLEX_SR,
            ARAM
        };

        // https://pkg.go.dev/github.com/Kinveil/Riot-API-Golang/constants/queue
        static const std::array<queue_t, 5> queues = {
            queue_t{ 400, "NORMAL_DRAFT" },
            queue_t{ 420, "RANKED_SOLO_5x5" },
            queue_t{ 430, "NORMAL_BLIND" },
            queue_t{ 440, "RANKED_FLEX_SR" },
            queue_t{ 450, "ARAM" }
        };

        inline std::optional<std::reference_wrapper<const queue_t>> get_queue_by_id(uint64_t id) {
            return get_by(queues, [&id](const queue_t& queue) { return queue.id == id; });
        }

        inline std::optional<std::reference_wrapper<const queue_t>> get_queue_by_name(const std::string& name) {
            return get_by(queues, [&name](const queue_t& queue) { return queue.name == name; });
        }
    }; // namespace queues

    namespace spells {
        struct spell_t {
            uint64_t id;
            std::string key;
            std::string name;
        };

        enum spell_array_index_t {
            SAI_BARRIER = 0,
            SAI_CLEANSE,
            SAI_EXHAUST,
            SAI_FLASH,
            SAI_GHOST,
            SAI_HEAL,
            SAI_SMITE,
            SAI_TELEPORT,
            SAI_CLARITY,
            SAI_IGNITE,
            SAI_MARK
        };
        
        static const std::array<spell_t, 11> spells = {
            spell_t{ 0, "SummonerBarrier", "Barrier" },
            spell_t{ 1, "SummonerBoost", "Cleanse" },
            spell_t{ 3, "SummonerExhaust", "Exhaust" },
            spell_t{ 4, "SummonerFlash", "Flash" },
            spell_t{ 6, "SummonerHaste", "Ghost" },
            spell_t{ 7, "SummonerHeal", "Heal" },
            spell_t{ 11, "SummonerSmite", "Smite" },
            spell_t{ 12, "SummonerTeleport", "Teleport" },
            spell_t{ 13, "SummonerMana", "Clarity" },
            spell_t{ 14, "SummonerDot", "Ignite" },
            spell_t{ 32, "SummonerSnowball", "Mark" }
        };

        inline std::optional<std::reference_wrapper<const spell_t>> get_spell_by_id(uint64_t id) {
            return get_by(spells, [&id](const spell_t& spell) { return spell.id == id; });
        }

        inline std::optional<std::reference_wrapper<const spell_t>> get_spell_by_key(const std::string& key) {
            return get_by(spells, [&key](const spell_t& spell) { return spell.key == key; });
        }

        inline std::optional<std::reference_wrapper<const spell_t>> get_spell_by_name(const std::string& name) {
            return get_by(spells, [&name](const spell_t& spell) { return spell.name == name; });
        }
    } // namespace spells
} // namespace league_constants

namespace lc = league_constants;

class league_connector_manager : public manager::abstract {
public:
    bool setup() override;
    bool shutdown() override;

    void add_connect_handler(const vu_function<void>& callback);
    void add_disconnect_handler(const vu_function<void>& callback);
    void add_endpoint_callback(const std::string& endpoint, vu_function<void, std::string, nlohmann::json> callback);

    template <int _return_code = 200>
    std::optional<nlohmann::json> request(connector::request_type type, const std::string& endpoint, const std::string& data = "") {
        const auto result = connector::MakeRequest(type, endpoint, data);
        return result.status == _return_code
            ? std::optional(result.data)
            : std::nullopt;
    }

private:
    bool m_is_setup = false;
    std::vector<vu_function<void>> m_connect_handlers {};
    std::vector<vu_function<void>> m_disconnect_handlers {};
};

namespace league_helpers {
    lpp_c_u_no_discard inline bool dodge_queue() {
        auto lcm = manager::instance<league_connector_manager>();
        return lcm->request<200>(connector::request_type::POST, "/lol-login/v1/session/invoke?destination=lcdsServiceProxy&method=call&args=[\"\",\"teambuilder-draft\",\"quitV2\",\"\"]", "[\"\",\"teambuilder-draft\",\"quitV2\",\"\"]").has_value();
    }

    lpp_c_u_no_discard inline bool accept_match() {
        auto lcm = manager::instance<league_connector_manager>();
        return lcm->request<204>(connector::request_type::POST, "/lol-matchmaking/v1/ready-check/accept").has_value();
    }

    lpp_c_u_no_discard inline std::optional<lc::kv_t> get_assigned_position(const champselect::Session& session) {
        auto it = std::ranges::find_if(session.myTeam.value(), [&session](const champselect::MyTeam& obj) {
            return obj.cellId.value() == session.localPlayerCellId.value();
        });

        if (it == session.myTeam->end())
            return std::nullopt;

        return lc::get_struct(it->assignedPosition.value(), lc::lanes::list);
    }

    lpp_c_u_no_discard inline bool start_search() {
        auto lcm = manager::instance<league_connector_manager>();
        return lcm->request<204>(connector::request_type::POST, "/lol-lobby/v2/lobby/matchmaking/search").has_value();
    }

    lpp_c_u_no_discard inline bool s_clear_tokens() {
        auto lcm = manager::instance<league_connector_manager>();
        
        auto player_data_request = lcm->request<200>(connector::request_type::GET, "/lol-challenges/v1/summary-player-data/local-player");
        if (!player_data_request.has_value())
            return false;

        auto player_data = player_data_request->get<challenges::SummaryPlayerData>();
        const auto& banner_id = player_data.bannerId.value();
        const auto& title_id = player_data.title->itemId.value();

        return lcm->request<204>(connector::request_type::POST, "/lol-challenges/v1/update-player-preferences", "{\"bannerAccent\":\"" + banner_id + "\",\"challengeIds\":[],\"title\":\"" + std::to_string(title_id) + "\"}").has_value();
    }
} // namespace league_helpers

namespace lh = league_helpers;

#endif // __LEAGUE_CONNECTOR_MANAGER_HPP__