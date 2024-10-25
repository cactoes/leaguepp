#pragma once

#ifndef __FEATURE_PROFILE_CONTROLLER_HPP__
#define __FEATURE_PROFILE_CONTROLLER_HPP__

#include "../managers/feature_manager.hpp"

namespace feature {
    class profile_controller : public abstract_feature {
    public:
        void setup(std::shared_ptr<vui::component::abstract_frame> frame) override;

    private:
        std::shared_ptr<vui::component::abstract_button> m_button_update_profile = nullptr;
        std::shared_ptr<vui::component::abstract_button> m_button_clear_tokens = nullptr;
    };
} // namespace feature

#endif // __FEATURE_PROFILE_CONTROLLER_HPP__