#ifndef __LOBBYCONTROLLS_HPP__
#define __LOBBYCONTROLLS_HPP__

#include "feature.hpp"

namespace feature {
    class LobbyControlls : public Feature {
    public:
        void Setup(std::shared_ptr<ui::Frame> frame) override;
        std::string GetName() override;
    };
}; // namespace feature

#endif // __LOBBYCONTROLLS_HPP__