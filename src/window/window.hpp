#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <Windows.h>

namespace browser {
    class js_args_t;
    class browser;
};

namespace window {
    HWND Create(bool isResizeable, bool hideTitleBar, bool show, int width, int height, int startX, int startY, const char* name, int icon, browser::browser* handle);
    LRESULT HandleWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void Destroy(HWND handle);
    
    bool Init(browser::browser* handle);
    bool HandleWindowEvent(browser::browser* handle, browser::js_args_t args);
}; // namespace window

#endif // __WINDOW_HPP__