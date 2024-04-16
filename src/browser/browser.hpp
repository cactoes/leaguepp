#ifndef __BROWSER_HPP__
#define __BROWSER_HPP__

#include <Windows.h>
#include <stdint.h>
#include <vector>
#include <any>
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>

#include "webview.hpp"
#include "eventbus.hpp"

#define SET_FLAG(_F, _V) ((_F) |= (_V))
#define CLEAR_FLAG(_F, _V) ((_F) &= ~(_V))
#define HAS_FLAG(_F, _V) (((_F) & (_V)) != 0)

#define CREATE_REGISTRATION(func) #func, &func
#define CREATE_REGISTRATION_MEMBER(func) &func, this

namespace browser {
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

    class js_value {
    public:
        js_value(const std::any& v) : m_value(v) {};

        template <typename Ty>
        std::enable_if_t<is_valid_type_v<Ty>, Ty>
        As() {
            return std::any_cast<Ty>(m_value);
        }

        std::vector<js_value> AsArray() {
            return std::any_cast<std::vector<js_value>>(m_value);
        }
        
    private:
        std::any m_value;
    };

    class js_args_t {
    public:
        void Push(const js_value& value) {
            m_args.push_back(value);
        }
        
        js_value& At(std::size_t index) {
            return m_args.at(index);
        }

        template <std::size_t I, typename Ty>
        std::enable_if_t<is_valid_type_v<Ty>, Ty>
        Get() {
            return m_args.at(I).As<Ty>();
        }

        template <std::size_t I>
        std::vector<js_value> GetArray() {
            return m_args.at(I).AsArray();
        }

        size_t Size() {
            return m_args.size();
        }

    private:
        std::vector<js_value> m_args = {};
    };

    // !!! having WF_HIDE_TITLE_BAR & WF_RESIZEABLE
    // at the same time will leave a visible resize handle at the top of the window
    // all the other sides are transparent
    enum window_flags {
        WF_RESIZEABLE = 1 << 0,
        WF_HIDE_TITLE_BAR = 1 << 1,
        WF_SHOW = 1 << 2
    };

    enum browser_flags {
        BF_ALLOW_DEV_TOOLS = 1 << 0,
        BF_ENABLE_DEBUG_CONSOLE = 1 << 1
    };

    struct window_config_t {
        int width = 700;
        int height = 600;
        int startX = 0;
        int startY = 0;

        int icon = -1;

        const char* name = "browser";
        uint64_t flags = 0ULL;
    };
    
    struct browser_config_t {
        const char* relViewPath = "view";
        uint64_t flags = 0ULL;
    };

    class Browser {
    public:
        // blocking. start the browser and wait until the program closes
        void Start();

        void RegisterFunction(const std::string& name, std::function<std::any(Browser*, js_args_t)> func);

        template <typename Ty>
        std::enable_if_t<is_valid_type_v<Ty>, void>
        RegisterFunction(const std::string& name, Ty(*func)(Browser*, js_args_t)) {
            RegisterFunction(name, [func](Browser* handle, js_args_t args) {
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
        RegisterFunction(const std::string& name, Ty(C::*func)(Browser*, js_args_t), C* obj) {
            RegisterFunction(name, [obj, func](Browser* handle, js_args_t args) {
                if constexpr (std::is_void_v<Ty>) {
                    (obj->*func)(handle, args);
                    return std::any{};
                } else {
                    return std::any{ (obj->*func)(handle, args) };
                }
            });
        }

        void CallJSFunction(const std::string& name, const std::vector<std::any>& args);
        void AddDOMContentLoadedHandler(std::function<void(Browser*)> func);

        void MoveWindow(Browser*, js_args_t jsArgs);
        void OnDomContentLoaded();

    public:
        HWND m_windowHandle;
        EventBus m_bus = {};
        
        wil::com_ptr<ICoreWebView2Controller> m_webViewController = nullptr;
        wil::com_ptr<ICoreWebView2> m_webView2 = nullptr;
        wil::com_ptr<ICoreWebView2_2> m_webView2_2 = nullptr;

        std::unordered_map<std::string, std::function<std::any(Browser*, js_args_t)>> m_methodMap = {};
        std::unordered_map<std::string, std::vector<std::function<void(Browser*)>>> m_eventMethodMap = {};

        bool m_isReady = false;
    };

    std::shared_ptr<Browser> CreateBrowser(const browser_config_t& browserConfig, const window_config_t& windowConfig);
}; // namespace browser

#endif // __BROWSER_HPP__