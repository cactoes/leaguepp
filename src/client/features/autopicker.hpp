#ifndef __AUTOPICKER_HPP__
#define __AUTOPICKER_HPP__

#include <vector>
#include <memory>

#include "feature.hpp"

namespace champselect {
    struct Session;
}; // namespace champselect

namespace lobby {
    struct Lobby;
}; // namespace lobby

class Config;

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
        INVALID = 0,

        // when are we declaring?
        DECLARING,
        
        BANNING,
        PICKING,
        WAITING
    };

    enum class lane_state {
        INVALID = 0,
        // only valid when in:
        // - solo/duo
        // - flex
        // - draft
        ASSIGNED_PRIMARY_POSITION,
        ASSIGNED_SECONDARY_POSITION,

        // assume when there are no lanes to pre pick
        // so like draft mode etc
        NO_PICKABLE_LANE,

        // assumed you got autofilled elsewhere
        OTHER_POSITION
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

    struct lobby_info_t {
        std::vector<int> lockedChampions = {};
        // gets set in gameflow hook
        bool isInChampSelect = false;
    };

    class AutoPicker : public IFeature {
    public:
        void Setup(std::shared_ptr<ui::Frame> frame, IUiFramework* frameworkApiHandle) override;
        std::string GetName() override;

    private:
        std::vector<int> GetLockedChampions(const champselect::Session& session);
        player_state GetPlayerState(const champselect::Session& session);
        lane_state GetLaneState(const champselect::Session& session, const lobby::Lobby& lobby);
        bool ValidateLaneState(lane_state state, int strictness);
        
        void HandleFrame(const champselect::Session& session, const lobby::Lobby& lobby);
        int MakeAction(const champselect::Session& session, action_type type, const std::vector<int> options, bool commit);
        int GetNextPick(const std::vector<int>& list);
        bool DoAction(int actionId, int championId, bool commit);

    private:
        const std::vector<std::string> m_modes = { "manual", "semi", "auto" };
        const std::vector<std::string> m_strictnesses = { "none", "loose", "strict" };
        const std::vector<std::string> m_lanes = { "top", "jungle", "middle", "bottom", "utility" };
        
        lobby_info_t m_lobby_info = {};
        std::shared_ptr<Config> m_config = nullptr;
        IUiFramework* m_frameworkApiHandle = nullptr;
        std::string m_listTarget;
    };
}; // namespace feature

#endif // __AUTOPICKER_HPP__