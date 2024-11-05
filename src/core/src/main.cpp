#include <reflection/reflection.hpp>

#include "managers/window_manager.hpp"
#include "managers/league_connector_manager.hpp"
#include "managers/config_manager.hpp"
#include "managers/feature_manager.hpp"
#include "managers/resource_manager.hpp"
#include "managers/log_manager.hpp"
#include "managers/manager.hpp"

#include "features/lobby_controller.hpp"
#include "features/profile_controller.hpp"
#include "features/champion_select_controller.hpp"

#define IDI_ICON_DEFAULT 1
#define IDI_ICON_DISCONNECTED 2
#define IDI_ICON_CONNECTED 3

void init_config() {
    std::shared_ptr<config_manager> cm = manager::instance<config_manager>();
    config_handle _config_handle = cm->create_config(USER_SETTINGS_CONFIG);
    
    _config_handle->add_template<bool>("b_auto_accept");
    _config_handle->add_template<bool>("b_auto_dodge");
    _config_handle->add_template<bool>("b_auto_search");
    _config_handle->add_template<std::vector<int>>("vec_auto_dodge_lanes");

    _config_handle->add_template<std::string>("str_tier");
    _config_handle->add_template<std::string>("str_division");
    _config_handle->add_template<std::string>("str_mastery");
    _config_handle->add_template<bool>("b_auto_update_profile");

    _config_handle->add_template<feature::champion_select_controller::mode_t>("e_csc_state_manager_mode");
    _config_handle->add_template<feature::champion_select_controller::strictness_t>("e_csc_state_manager_strictness");
    _config_handle->add_template<int>("i_pref_line_blind");
    _config_handle->add_template<bool>("b_auto_picker_enabled");
    _config_handle->add_template<bool>("b_early_declare_enabled");

    for (auto& lane : lc::lanes::list) {
        _config_handle->add_template<std::vector<int64_t>>("vec_csc_picks_" + std::to_string(lane.index));
        _config_handle->add_template<std::vector<int64_t>>("vec_csc_bans_" + std::to_string(lane.index));
    }

    if (!cm->load_config(_config_handle))
        cm->dump_config(_config_handle);
}

void init_window_manager() {
    auto _window_manager = manager::instance<wm>();
    _window_manager->setup();

    auto main_frame = _window_manager->get_window()->get_frame();

    auto left_frame = main_frame->add_frame("", { .outline = false, .max_size = true });
    left_frame->set_id("__left_frame");

    auto right_frame = main_frame->add_frame("", { .outline = false, .max_size = true });
    right_frame->set_id("__right_frame");

    auto csc_frame = left_frame->add_frame("Champion select controller", { .outline = true, .max_size = true });
    csc_frame->set_id("__csc_frame");

    auto pc_frame = right_frame->add_frame("Profile controller", { .outline = true, .max_size = false });
    pc_frame->set_id("__pc_frame");

    auto lc_frame = right_frame->add_frame("Lobby controller", { .outline = true, .max_size = false });
    lc_frame->set_id("__lc_frame");
    
    auto logs_frame = right_frame->add_frame("Logs", { .max_size = true });
    logs_frame->set_id("__logger_frame");

    // std::string m_current_color = "#728ab3";
    // frame1_1_parent->add_input(m_current_color, [&](auto, std::string v) {
    //     m_current_color = lpp_core_utils::test(v, "^#([a-fA-F0-9]{6})$") ? v : m_current_color;
    //     window->set_color(m_current_color);
    //     return m_current_color;
    // });
}

void init_log_manager() {
    auto _log_manager = manager::instance<log_manager>();
    auto _window_manager = manager::instance<wm>();

    auto logs_frame = _window_manager->get_window()->get_frame()
        ->get_component<reflection::component::abstract_frame>("__right_frame").value()
        ->get_component<reflection::component::abstract_frame>("__logger_frame").value();

    _log_manager->setup_frame(logs_frame);
}

void init_league_connector_manager() {
    auto lcm = manager::instance<league_connector_manager>();

    lcm->add_connect_handler([]() {
        auto wm_window = manager::instance<wm>()->get_window();
        auto _log_manager = manager::instance<log_manager>();

        wm_window->set_icon(IDI_ICON_CONNECTED);
        _log_manager->add_log("[lcm] Connected to league");
    });

    lcm->add_disconnect_handler([]() {
        auto wm_window = manager::instance<wm>()->get_window();
        auto _log_manager = manager::instance<log_manager>();

        wm_window->set_icon(IDI_ICON_DISCONNECTED);
        _log_manager->add_log("[lcm] Disconnected from league");
    });
}

void init_feature_manager() {
    auto fm = manager::instance<feature_manager>();

    auto frame_main = manager::instance<wm>()->get_window()->get_frame();
    auto left_frame = frame_main->get_component<reflection::component::abstract_frame>("__left_frame").value();
    auto right_frame = frame_main->get_component<reflection::component::abstract_frame>("__right_frame").value();

    fm->add_feature<feature::lobby_controller>(right_frame->get_component<reflection::component::abstract_frame>("__lc_frame").value());
    fm->add_feature<feature::profile_controller>(right_frame->get_component<reflection::component::abstract_frame>("__pc_frame").value());
    fm->add_feature<feature::champion_select_controller>(left_frame->get_component<reflection::component::abstract_frame>("__csc_frame").value());
}

void lppmain() {
    // ~~~ order matters!

    // init independent managers
    init_config();
    manager::instance<resource_manager>()->setup();

    // init dependent managers
    init_window_manager();
    init_log_manager();
    init_league_connector_manager();
    init_feature_manager();

    // handle window events etc
    auto wm_window = manager::instance<wm>()->get_window();

    wm_window->register_event_handler(reflection::event_t::E_ON_RENDER_FINISHED, [](auto) {
        manager::instance<league_connector_manager>()->setup();
    });

    wm_window->start();

    MSG msg{};
    while (GetMessageA(&msg, nullptr, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    // only lcm requires cleanup
    manager::instance<league_connector_manager>()->shutdown();
}

int main(int, char**) {
    lppmain();
    return 0;
}

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    lppmain();
    return 0;
}