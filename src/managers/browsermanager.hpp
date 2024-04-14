#ifndef __BROWSERMANAGER_HPP__
#define __BROWSERMANAGER_HPP__

#include <memory>

#include "../browser/browser.hpp"

enum class notification_type {
    NONE,
    ERR,
    WARNING,
    SUCCESS
};

class browser_manager {
public:
    void Setup();
    void Start();

    browser::browser* GetHandle();

    void CallJS(const std::string& name, const std::vector<std::any>& args);
    void CreateNotification(const std::string& title, const std::string& description, notification_type type);

private:
    std::shared_ptr<browser::browser> m_handle = nullptr;
};

#endif // __BROWSERMANAGER_HPP__