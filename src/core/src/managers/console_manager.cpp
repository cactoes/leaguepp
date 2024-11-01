#include "managers/log_manager.hpp"

void log_manager::setup_frame(std::shared_ptr<reflection::component::abstract_frame> frame) {
    frame->add_label("(Higher is newer)");
    m_console_list = frame->add_list("", {}, [this](auto, int clicked_item) {
        m_logs.erase(m_logs.begin() + clicked_item);
        m_console_list->set_items(m_logs);
    }).value();
}

void log_manager::add_log(const std::string& message) {
    m_logs.insert(m_logs.begin(), message);
    m_console_list->set_items(m_logs);
}