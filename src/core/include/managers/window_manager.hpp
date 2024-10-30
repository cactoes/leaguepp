#pragma once

#ifndef __WINDOW_MANAGER_HPP__
#define __WINDOW_MANAGER_HPP__

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>
#include <reflection/reflection.hpp>

#define COLOR_SCEME_HEX "#728ab3"

namespace window_manager {
    LRESULT CALLBACK handle_window_message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    struct window_options_t {
        std::string name = "window";
        int width = 700;
        int height = 700;

        // 0 is center
        int pos_x = 0;
        // 0 is center
        int pos_y = 0;
    };

    struct window_options_ex_t : window_options_t {
        int icon_id = 1;
        HINSTANCE instance = nullptr;
        std::string class_name = "wnd_cls";
    };

    HWND create_window(const window_options_t& options);
    HWND create_window_ex(const window_options_ex_t& options_ex);

    std::shared_ptr<reflection::browser_window> create_window(const reflection::component::frame_options_t& frame_options, const window_options_t& window_options);
    std::shared_ptr<reflection::browser_window> create_window(const reflection::component::frame_options_t& frame_options, const window_options_ex_t& window_options_ex);

    void create_notification(const std::string& title, const std::string& message);

    namespace detail {
        struct _nc_req_t {
            std::string title;
            std::string message;
        };

        inline std::thread::id g_main_thread_id = std::this_thread::get_id();
        inline std::mutex g_nc_req_lock {};
        inline std::queue<_nc_req_t> g_nc_req_queue {};

        inline void post_nc_req(const _nc_req_t& nc_req) {
            std::lock_guard<std::mutex> guard(g_nc_req_lock);
            g_nc_req_queue.push(nc_req);
        }

        inline bool get_nc_req(_nc_req_t& nc_req) {
            std::lock_guard<std::mutex> guard(g_nc_req_lock);

            if (g_nc_req_queue.empty())
                return false;

            nc_req = g_nc_req_queue.front();
            g_nc_req_queue.pop();

            return true;
        }
    } // namespace detail
} // namespace window_manager



namespace wm = window_manager;

#endif // __WINDOW_MANAGER_HPP__