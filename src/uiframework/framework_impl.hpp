#ifndef __FRAMEWORK_IMPL_HPP__
#define __FRAMEWORK_IMPL_HPP__

#include "include/framework.hpp"
#include "browser/browser.hpp"
#include "include/tab.hpp"

class UiFrameworkImpl : public IUiFramework {
public:
    bool Init() override;
    void Shutdown() override;

    void Run() override;

    void CreateNotification(const std::string& title, const std::string& description) override;

    std::vector<std::shared_ptr<ui::Tab>> GetTabs() override;
    std::shared_ptr<ui::Tab> AddTab(const std::string& label, bool isActive = false) override;

    void GetMainLayout(browser::Browser*, browser::js_args_t);

private:
    std::vector<std::shared_ptr<ui::Tab>> m_tabs;
    std::shared_ptr<browser::Browser> m_browserHandle;
};

#endif // __FRAMEWORK_IMPL_HPP__