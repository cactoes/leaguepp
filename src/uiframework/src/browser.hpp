#pragma once

#ifndef __BROWSER_HPP__
#define __BROWSER_HPP__

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <wil/com.h>
#include <wrl.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

#include <unordered_map>
#include <string>
#include <mutex>
#include <vector>
#include <queue>
#include <any>

#include "window.hpp"

using Microsoft::WRL::Make, Microsoft::WRL::Callback, Microsoft::WRL::ComPtr;

class EventManager {
public:
    struct EVENT {
        std::string name;
        std::vector<std::any> args;
    };

    void Add(const EVENT& event) {
        std::lock_guard<std::mutex> guard(m_mutex);
        m_bus.push(event);
    }

    bool Get(EVENT& event) {
        std::lock_guard<std::mutex> guard(m_mutex);

        if (m_bus.empty())
            return false;

        event = m_bus.front();
        m_bus.pop();

        return true;
    }

private:
    std::queue<EVENT> m_bus = {};
    std::mutex m_mutex;
};

template<typename Ty>
struct is_valid_type : std::disjunction<
    std::is_same<Ty, int>,
    std::is_same<Ty, float>,
    std::is_same<Ty, bool>,
    std::is_same<Ty, std::string>,
    std::is_same<Ty, void>,
    std::is_same<Ty, std::vector<std::any>>
> {};

template<typename Ty>
constexpr bool is_valid_type_v = is_valid_type<Ty>::value;

class JSValue {
public:
    JSValue(const std::any& v) : m_value(v) {};

    template <typename Ty>
    std::enable_if_t<is_valid_type_v<Ty>, Ty>
    As() {
        return std::any_cast<Ty>(m_value);
    }

    std::vector<JSValue> AsArray() {
        return std::any_cast<std::vector<JSValue>>(m_value);
    }
    
private:
    std::any m_value;
};

typedef std::vector<JSValue> JSArgs;

class Browser {
public:
    Browser(const Window& window, const BROWSER_CONFIG& config);
    
    static LRESULT OnWindowMessage(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void CallJSFunction(const std::string& name, const std::vector<std::any>& args);
    void AddDOMContentLoadedHandler(std::function<void(Browser*)> func);
    void RegisterFunction(const std::string& name, std::function<std::any(Browser*, JSArgs)> func);

    template <typename Ty>
    std::enable_if_t<is_valid_type_v<Ty>, void>
    RegisterFunction(const std::string& name, Ty(*func)(Browser*, JSArgs)) {
        RegisterFunction(name, [func](Browser* handle, JSArgs args) {
            if constexpr (std::is_void_v<Ty>) {
                func(handle, args);
                return std::any{};
            } else {
                return std::any{ func(handle, args) };
            }
        });
    }

    template <typename Ty, typename C>
    std::enable_if_t<is_valid_type_v<Ty>, void>
    RegisterFunction(const std::string& name, Ty(C::*func)(Browser*, JSArgs), C* obj) {
        RegisterFunction(name, [obj, func](Browser* handle, JSArgs args) {
            if constexpr (std::is_void_v<Ty>) {
                (obj->*func)(handle, args);
                return std::any{};
            } else {
                return std::any{ (obj->*func)(handle, args) };
            }
        });
    }

    void EmptyEventBus(void);

private:
    void MoveWindow(Browser*, JSArgs args);
    bool HandleWindowEvent(Browser*, JSArgs args);
    void OnDomContentLoaded();

private:
    Window m_window;
    EventManager m_eventManager = {};
    
    wil::com_ptr<ICoreWebView2Controller> m_webViewController = nullptr;
    wil::com_ptr<ICoreWebView2> m_webView2 = nullptr;
    wil::com_ptr<ICoreWebView2_2> m_webView2_2 = nullptr;

    bool m_isReady = false;

    std::unordered_map<std::string, std::function<std::any(Browser*, JSArgs)>> m_methodMap = {};
    std::unordered_map<std::string, std::vector<std::function<void(Browser*)>>> m_eventMethodMap = {};
};

#endif // __BROWSER_HPP__