#ifndef __BROWSERMANAGER_HPP__
#define __BROWSERMANAGER_HPP__

#include <memory>

#include "../browser/browser.hpp"

class browser_manager {
public:
    void Setup();
    void Start();

    browser::browser* GetHandle();

private:
    std::shared_ptr<browser::browser> m_handle = nullptr;
};

#endif // __BROWSERMANAGER_HPP__