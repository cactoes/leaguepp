#ifndef __AUTOPICKER_HPP__
#define __AUTOPICKER_HPP__

#include <vector>
#include <memory>

#include "feature.hpp"
#include "../ui/checkbox.hpp"
#include "endpointmappers/champselectsession.hpp"
#include "endpointmappers/lobbylobby.hpp"
#undef interface

class config;

// ref madoka

namespace feature {
    // https://static.developer.riotgames.com/docs/lol/queues.json
    enum queue_type {
        QT_INVALID = -1,
        QT_DRAFT = 400,
        QT_SOLO_DUO = 420,
        QT_FLEX = 440
    };

    enum class player_state {
        INVALID,

        // when are we declaring?
        DECLARING,
        
        BANNING,
        PICKING,
        WAITING
    };

    enum class lane_state {
        INVALID,
        // only valid when in:
        // - solo/duo
        // - flex
        // - draft
        ASSIGNED_PRIMARY_POSITION = 0,
        ASSIGNED_SECONDARY_POSITION,

        // assume when there are no lanes to pre pick
        // so like draft mode etc
        NO_PICKABLE_LANE,

        // assumed you got autofilled elsewhere
        OTHER_POSITION
    };

    struct lobby_state_t {
        std::vector<int64_t> lockedChampions = {};
        player_state playerState = player_state::INVALID;
        lane_state laneState = lane_state::INVALID;
        bool isLaneValid = false;

        // gets set in gameflow hook
        bool isInChampSelect = false;
    };

    enum bot_mode {
        // here the bot is disabled so we dont do anything
        BOT_MANUAL = 0,

        // we only declare but never actually commit
        BOT_SEMI,

        // full commit to the picks & bans
        BOT_AUTO
    };

    enum class action_type {
        PICK,
        BAN
    };

    class auto_picker : public feature {
    public:
        void Setup(std::shared_ptr<ui::frame> frame) override;
        std::string GetName() override;

    private:
        void UpdateLobbyState(const champselect::Session& data, int strictness);
        
        void RunAutoPicker(const champselect::Session& data, std::shared_ptr<config> cfg);
        bool MakeAction(const champselect::Session& data, action_type type, bool commit, const std::vector<int> options);
        int GetNextPick(const std::vector<int>& list);

        player_state GetPlayerState(const champselect::Session& data);
        lane_state GetLaneState(const champselect::Session& session, const lobby::Lobby& lobby);
        bool ValidateLaneState(int strictness);

        bool DoAction(int actionId, int championId, bool commit);

    private:
        const std::vector<std::string> m_modes = { "manual", "semi", "auto" };
        const std::vector<std::string> m_strictnesses = { "none", "loose", "strict" };
        lobby_state_t m_lobby_state = {};
    };
}; // namespace feature

#endif // __AUTOPICKER_HPP__