#ifndef __AUTOPICKER_HPP__
#define __AUTOPICKER_HPP__

#include "feature.hpp"
#include "../ui/checkbox.hpp"
#undef interface

// ref madoka

namespace feature {
    class auto_picker : public feature {
    public:
        void Setup(std::shared_ptr<ui::frame> frame) override;
        std::string GetName() override;

    private:
        const std::vector<std::string> m_modes = { "manual", "semi", "auto" };
    };
}; // namespace feature

#endif // __AUTOPICKER_HPP__