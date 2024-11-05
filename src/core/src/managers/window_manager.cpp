#include "managers/window_manager.hpp"

#include <reflection/reflection.hpp>
#include <map>
#include <iostream>

std::map<HWND, std::shared_ptr<reflection::browser_window>> g_windows {};
HWND g_main_window = nullptr;

LRESULT CALLBACK window_manager::handle_window_message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (reflection::handle_window_message(hWnd, msg, wParam, lParam))
        return TRUE;

    switch (msg) {
        case (WM_USER + 0x0003): {
            detail::_nc_req_t req {};
            while (detail::get_nc_req(req))
                create_notification(req.title, req.message);
            return TRUE;
        }
        case WM_HANDLE_WINDOW_CREATE:
            if (!g_main_window)
                g_main_window = hWnd;
            return TRUE;
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return TRUE;
        case WM_DESTROY:
            if (g_main_window == hWnd) {
                for (auto& [ _hwnd, _bp ] : g_windows)
                    _bp->close();
                PostQuitMessage(0);
                return TRUE;
            }

            g_windows.erase(hWnd);
            if (g_windows.size() <= 0)
                PostQuitMessage(0);
            return TRUE;
        default:
            break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND window_manager::create_window(const window_options_t& options) {
    window_options_ex_t _window_options_ex { options };
    return create_window_ex(_window_options_ex);
}

HWND window_manager::create_window_ex(const window_options_ex_t& options_ex) {
    // const HICON icon = LoadIconA(options_ex.instance, MAKEINTRESOURCE(options_ex.icon_id));
    const HICON icon = NULL;

    WNDCLASSEX window_class{};
    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.style = (CS_HREDRAW | CS_VREDRAW);
    window_class.lpfnWndProc = &handle_window_message;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = options_ex.instance;
    window_class.hCursor = LoadCursorA(options_ex.instance, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    window_class.lpszMenuName = nullptr;
    window_class.lpszClassName = options_ex.class_name.c_str();
    window_class.hIcon = icon;
    window_class.hIconSm = icon;

    RegisterClassExA(&window_class);
    
    const DWORD window_style_flags = WS_POPUP | WS_VISIBLE;

    const int window_pos_x = options_ex.pos_x ? options_ex.pos_x : (GetSystemMetrics(SM_CXSCREEN) - options_ex.width) / 2;
    const int window_pos_y = options_ex.pos_y ? options_ex.pos_y : (GetSystemMetrics(SM_CYSCREEN) - options_ex.height) / 2;

    const HWND window_handle = CreateWindowExA(
        0,
        options_ex.class_name.c_str(),
        options_ex.name.c_str(),
        window_style_flags,
        window_pos_x, window_pos_y,
        options_ex.width, options_ex.height,
        NULL, NULL,
        options_ex.instance,
        NULL);

    UpdateWindow(window_handle);

    return window_handle;
}

std::shared_ptr<reflection::browser_window> window_manager::create_window(const reflection::component::frame_options_t& frame_options, const window_options_t& window_options) {
    window_options_ex_t _window_options_ex { window_options };
    return create_window(frame_options, _window_options_ex);
}

std::shared_ptr<reflection::browser_window> window_manager::create_window(const reflection::component::frame_options_t& frame_options, const window_options_ex_t& window_options_ex) {
    HWND hwnd = create_window_ex(window_options_ex);
    auto browser_window_ptr = std::make_shared<reflection::browser_window>(hwnd, frame_options);
    browser_window_ptr->set_color(COLOR_SCEME_HEX);
    g_windows[hwnd] = browser_window_ptr;
    return browser_window_ptr;
}

void window_manager::create_notification(const std::string& title, const std::string& message) {
    if (detail::g_main_thread_id != std::this_thread::get_id()) {
        detail::post_nc_req(detail::_nc_req_t{ title, message });
        PostMessageA(g_main_window, (WM_USER + 0x0003), 0, 0);
    } else {
        reflection::component::frame_options_t frame_options {};
        frame_options.layout = reflection::component::fl_vertical;
        frame_options.align = reflection::component::fa_center;

        window_options_t window_options {};
        window_options.name = title;
        window_options.width = 220;
        window_options.height = 130;

        auto browser_window = create_window(frame_options, window_options);

        auto frame = browser_window->get_frame();

        frame->add_label(message);
        frame->add_button("Ok", [browser_window](auto){ browser_window->close(); });

        browser_window->start();
    }
}

bool window_manager::window_manager::setup() {
    reflection::component::frame_options_t options {};
    options.layout = reflection::component::fl_horizontal;
    
    window_options_t window_options {};
    window_options.name = "League++";

    m_window = create_window(options, window_options);

    return true;
}

bool window_manager::window_manager::shutdown() {
    // no need to call
    // m_window->close();
    return true;
}

std::shared_ptr<reflection::browser_window> window_manager::window_manager::get_window() {
    return m_window;
}