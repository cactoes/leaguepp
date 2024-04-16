#ifndef __BROWSERMANAGER_HPP__
#define __BROWSERMANAGER_HPP__

#include <memory>
#include <any>
#include <string>
#include <vector>

namespace browser {
    class Browser;
}; // namespace browser

enum class notification_type {
    NONE,
    ERR,
    WARNING,
    SUCCESS
};

class BrowserManager {
public:
    bool Init();
    void Start();

    browser::Browser* GetHandle();

    void CallJS(const std::string& name, const std::vector<std::any>& args);
    void CreateNotification(const std::string& title, const std::string& description, notification_type type);

private:
    std::shared_ptr<browser::Browser> m_handle = nullptr;
};

#endif // __BROWSERMANAGER_HPP__