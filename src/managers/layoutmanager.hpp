#ifndef __LAYOUTMANAGER_HPP__
#define __LAYOUTMANAGER_HPP__

#include <memory>

namespace ui {
    class Frame;
}; // namespace ui

namespace browser {
    class js_args_t;
    class Browser;
}; // namespace browser

class LayoutManager {
public:
    bool Init();

    void PushLayout(browser::Browser* handle, browser::js_args_t args);

    std::shared_ptr<ui::Frame> GetFrame();

private:
    std::shared_ptr<ui::Frame> m_frameMain;
};

#endif // __LAYOUTMANAGER_HPP__