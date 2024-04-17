#ifndef __FRAMEWORK_IMPL_HPP__
#define __FRAMEWORK_IMPL_HPP__

#include "include/framework.hpp"
#include "browser/browser.hpp"

class UiFrameworkImpl : public IUiFramework {
public:
    bool Init() override;
    void Shutdown() override;

    void Run() override;

    void CreateNotification(const std::string& title, const std::string& description) override;

    std::shared_ptr<ui::Frame> GetMainFrame() override;

    void GetMainLayout(browser::Browser* handle, browser::js_args_t);

private:
    std::shared_ptr<ui::Frame> m_main;
    std::shared_ptr<browser::Browser> m_browserHandle;
};

#endif // __FRAMEWORK_IMPL_HPP__