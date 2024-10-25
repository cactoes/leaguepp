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

        class state_machine {
        public:
            state_machine() {}

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

            void set_mode(mode_t mode);
            void set_strictness(strictness_t strictness);

            std::optional<action_t> get_current_state(const champselect::Session& session, const lobby::Lobby& lobby);

        private:
            lane_t get_lane_state(const champselect::Session& session, const lobby::Lobby& lobby);
            bool check_lane_state(lane_t lane, strictness_t strictness);
            phase_t get_phase(const champselect::Session& session);

        private:
            mode_t m_mode {};
            strictness_t m_strictness {};
        };

        void setup(std::shared_ptr<vui::component::abstract_frame> frame) override;

    private:
        const std::vector<std::string> m_mode = { "manual", "semi", "auto" };
        const std::vector<std::string> m_strictness = { "none", "loose", "strict" };
        state_machine m_state_machine {};
    };

    typedef champion_select_controller::state_machine sm;
} // namespace feature

#endif // __FEATURE_CHAMPION_SELECT_CONTROLLER_HPP__