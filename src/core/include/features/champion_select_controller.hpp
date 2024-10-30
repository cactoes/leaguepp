#pragma once

#ifndef __FEATURE_CHAMPION_SELECT_CONTROLLER_HPP__
#define __FEATURE_CHAMPION_SELECT_CONTROLLER_HPP__

#include "../managers/feature_manager.hpp"
#include "../endpoint_mappers.hpp"

namespace feature {
    class champion_select_controller : public abstract_feature {
    public:
        enum mode_t {
            M_MANUAL = 0,
            M_SEMI,
            M_AUTO
        };

        enum strictness_t {
            S_NONE = 0,
            S_LOOSE,
            S_STRICT
        };

        void setup(std::shared_ptr<reflection::component::abstract_frame> frame) override;

    private:
        std::vector<int> get_locked_champions(const champselect::Session& session);
        int get_next_id(const std::vector<int>& locks, const std::vector<int>& list);

        void handle_frame(const champselect::Session& session, const lobby::Lobby& lobby);

    private:
        const std::vector<std::string> m_mode = { "manual", "semi", "auto" };
        const std::vector<std::string> m_strictness = { "none", "loose", "strict" };
        // state_machine m_state_machine {};
    };

    namespace state_manager {
        enum phase_t {
            P_INVALID = 0,

            P_DECLARING,
            P_BANNING,
            P_PICKING,
            P_WAITING
        };

        enum lane_t {
            L_INVALID = 0,

            // only valid for now:
            // - solo/duo
            // - flex
            // - draft
            L_PRIMARY,
            L_SECONDARY,

            // filled
            L_OTHER,

            // assume when there are no lanes to pre pick
            // so like draft mode etc
            L_IMPOSSIBLE,
        };

        enum action_type_t {
            AT_PICK = 0,
            AT_BAN,
        };

        struct action_t {
            action_type_t type;
            bool commit;
        };

        std::optional<action_t> get_current_state(const champselect::Session& session, const lobby::Lobby& lobby, champion_select_controller::mode_t mode, champion_select_controller::strictness_t strictness);

        lane_t get_lane_state(const champselect::Session& session, const lobby::Lobby& lobby);
        bool check_lane_state(lane_t lane, champion_select_controller::strictness_t strictness);
        phase_t get_phase(const champselect::Session& session);
    } // namespace state_manager

    // typedef champion_select_controller::state_machine sm;
} // namespace feature

#endif // __FEATURE_CHAMPION_SELECT_CONTROLLER_HPP__