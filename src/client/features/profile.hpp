#ifndef __PROFILE_HPP__
#define __PROFILE_HPP__

#include <functional>

#include "feature.hpp"
#include "../managers/configmanager.hpp"

namespace lolchat {
    struct Me;
}; // namespace lolchat

namespace feature {
    class Profile : public IFeature {
    public:
        void Setup(std::shared_ptr<ui::Frame> frame, IUiFramework* frameworkApiHandle) override;
        std::string GetName() override;

    private:
        bool UpdateProfile(std::function<bool(lolchat::Me& me)> setter);

        std::vector<std::string> OnTierUpdate(std::string tier, bool, std::vector<std::string> list);
        std::vector<std::string> OnDivisionUpdate(std::string division, bool, std::vector<std::string> list);
        std::string OnMasteryScoreInput(std::string input);
        void OnClickUpdateProfile();
        bool OnSetAutoUpdateProfile(bool state);
        void OnClickClearTokens();

    private:
        struct {
            CVarHandle<std::string> profileTier;
            CVarHandle<std::string> profileDivision;
            CVarHandle<std::string> profileMastery;
            CVarHandle<bool> profileAutoSet;
        } m_cfg;

        IUiFramework* m_frameworkApiHandle;
    };
}; // namespace feature

#endif // __PROFILE_HPP__