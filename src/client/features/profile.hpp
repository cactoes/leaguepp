#ifndef __PROFILE_HPP__
#define __PROFILE_HPP__

#include <functional>

#include "feature.hpp"

namespace lolchat {
    struct Me;
}; // namespace lolchat

namespace feature {
    class Profile : public Feature {
    public:
        void Setup(std::shared_ptr<ui::Frame> frame, IUiFramework* frameworkApiHandle) override;
        std::string GetName() override;

    private:
        bool IsValidItem(const std::vector<std::string>& list, const std::string& value) const;
        bool UpdateProfile(std::function<bool(lolchat::Me& me)> setter);

    private:
        const std::vector<std::string> m_divisions = { "I", "II", "III", "IV" };
        const std::vector<std::string> m_tiers = { "iron", "bronze", "silver", "gold", "platinum", "emerald", "diamond", "master", "grandmaster", "challenger" };
    };
}; // namespace feature

#endif // __PROFILE_HPP__