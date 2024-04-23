#ifndef __LOBBYCONTROLLS_HPP__
#define __LOBBYCONTROLLS_HPP__

#include "feature.hpp"
#include "../managers/configmanager.hpp"

namespace feature {
    class LobbyControlls : public IFeature {
    public:
        void Setup(std::shared_ptr<ui::Frame> frame, IUiFramework* frameworkApiHandle) override;
        std::string GetName() override;

    private:
        void OnNewGameflow(const std::string& gameFlow);
        bool OnSetAutoAccept(bool state);
        void OnClickDodge();
        bool OnSetAutoDodge(bool state);
        std::vector<std::string> OnAutoDodgeLanesUpdate(std::string, bool, std::vector<std::string> list);

    private:
        struct {
            CVarHandle<bool> lobbyAutoAccept;
            CVarHandle<bool> lobbyAutoDodge;
            CVarHandle<std::vector<int>> lobbyAutoDodgeLanes;
        } m_cfg;

        IUiFramework* m_frameworkApiHandle;
    };
}; // namespace feature

#endif // __LOBBYCONTROLLS_HPP__