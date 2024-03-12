#ifndef __AUTOACCEPT_HPP__
#define __AUTOACCEPT_HPP__

#include "feature.hpp"
#include "../ui/checkbox.hpp"
#undef interface

namespace feature {
    class auto_accept : public feature {
    public:
        void Setup(std::shared_ptr<ui::frame> frame) override;
        std::string GetName() override;
    };
}; // namespace feature

#endif // __AUTOACCEPT_HPP__