#ifndef __EVENTMANAGER_HPP__
#define __EVENTMANAGER_HPP__

#include "../browser/browser.hpp"
#undef interface

class event_manager {
public:
    void Setup(browser::browser* handle);

    bool HandleWindowEvent(browser::browser* handle, browser::js_args_t args);
};

#endif // __EVENTMANAGER_HPP__