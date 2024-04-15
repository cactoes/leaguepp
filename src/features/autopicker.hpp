#ifndef __AUTOPICKER_HPP__
#define __AUTOPICKER_HPP__

#include "feature.hpp"
#include "../ui/checkbox.hpp"
#undef interface

#include <vector>

// ref madoka

namespace feature {
    enum player_state {
        DECLARING,
        BANNING,
        PICKING,
        WAITING
    };

    struct lobby_state_t {
        std::vector<int> lockedChampions = {};
        player_state playerState = player_state::WAITING;
        bool isInChampSelect = false;
    };

    class auto_picker : public feature {
    public:
        void Setup(std::shared_ptr<ui::frame> frame) override;
        std::string GetName() override;

    private:
        const std::vector<std::string> m_modes = { "manual", "semi", "auto" };
        const std::vector<std::string> m_strictnesses = { "none", "loose", "strict" };
        lobby_state_t m_lobby_state = {};
    };
}; // namespace feature

#endif // __AUTOPICKER_HPP__