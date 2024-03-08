#ifndef __LAYOUTMANAGER_HPP__
#define __LAYOUTMANAGER_HPP__

#include <memory>

#include "../ui/frame.hpp"
#include "../ui/button.hpp"
#include "../ui/label.hpp"
#include "../ui/checkbox.hpp"
#include "../ui/slider.hpp"
#include "../ui/dropdown.hpp"
#include "../ui/selector.hpp"

#include "../browser/browser.hpp"
#undef interface

class layout_manager {
public:
    void Setup(browser::browser* handle);

    void PushLayout(browser::browser* handle, browser::js_args_t args);

    std::shared_ptr<ui::frame> GetFrame();

private:
    std::shared_ptr<ui::frame> m_frameMain;
};

#endif // __LAYOUTMANAGER_HPP__