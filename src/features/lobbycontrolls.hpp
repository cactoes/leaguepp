#ifndef __LOBBYCONTROLLS_HPP__
#define __LOBBYCONTROLLS_HPP__

#include "feature.hpp"
#include "../ui/frame.hpp"
#undef interface

namespace feature {
    class lobby_controlls : public feature {
    public:
        void Setup(std::shared_ptr<ui::frame> frame) override;
        std::string GetName() override;
    };
}; // namespace feature

#endif // __LOBBYCONTROLLS_HPP__