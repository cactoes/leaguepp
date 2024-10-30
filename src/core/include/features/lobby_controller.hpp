#pragma once

#ifndef __FEATURE_LOBBY_CONTROLLER_HPP__
#define __FEATURE_LOBBY_CONTROLLER_HPP__

#include "../managers/feature_manager.hpp"

namespace feature {
    class lobby_controller : public abstract_feature {
    public:
        void setup(std::shared_ptr<reflection::component::abstract_frame> frame) override;

    private:
        void handle_auto_accept(uint64_t gameflow_hash);
        void handle_auto_dodge(uint64_t gameflow_hash);
        void handle_auto_search(uint64_t gameflow_hash);

        std::shared_ptr<reflection::component::abstract_button> m_button_dodge = nullptr;
    };
} // namespace feature

#endif // __FEATURE_LOBBY_CONTROLLER_HPP__