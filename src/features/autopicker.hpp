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

    struct lobby_info_t {
        std::vector<int> lockedChampions = {};
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
        std::vector<int> GetLockedChampions(const champselect::Session& session);
        player_state GetPlayerState(const champselect::Session& session);
        lane_state GetLaneState(const champselect::Session& session, const lobby::Lobby& lobby);
        bool ValidateLaneState(lane_state state, int strictness);
        
        void RunAutoPicker(const champselect::Session& session, const lobby::Lobby& lobby, std::shared_ptr<config> cfg);
        int MakeAction(const champselect::Session& session, action_type type, bool commit, const std::vector<int> options);
        int GetNextPick(const std::vector<int>& list);
        bool DoAction(int actionId, int championId, bool commit);

        void NotifyUser(const std::string& title, const std::string& message);

    private:
        const std::vector<std::string> m_modes = { "manual", "semi", "auto" };
        const std::vector<std::string> m_strictnesses = { "none", "loose", "strict" };
        lobby_info_t m_lobby_info = {};
    };
}; // namespace feature

#endif // __AUTOPICKER_HPP__