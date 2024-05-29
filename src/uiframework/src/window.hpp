#pragma once

#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "uiframework.hpp"

class Window {
public:
    Window(HWND window, const std::string& title)
        : m_window(window), m_title(title) {}

    static Window Create(const WINDOW_CONFIG& config);

    HWND GetWindowHandle(void) const { return m_window; }
    void Destroy(void) { DestroyWindow(m_window); }

    const std::string& GetTitle(void) const { return m_title; }

private:
    HWND m_window;
    std::string m_title;
};

#endif // __WINDOW_HPP__