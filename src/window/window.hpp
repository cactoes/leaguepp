#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <Windows.h>

namespace browser {
    class browser;
};

namespace window {
    HWND Create(bool isResizeable, bool hideTitleBar, bool show, int width, int height, int startX, int startY, const char* name, browser::browser* handle);
    LRESULT HandleWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void Destroy(HWND handle);
}; // namespace window

#endif // __WINDOW_HPP__