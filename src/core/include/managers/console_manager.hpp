#pragma once

#ifndef __CONSOLE_MANAGER_HPP__
#define __CONSOLE_MANAGER_HPP__

#include "manager.hpp"
#include <reflection/reflection.hpp>

class console_manager : public manager::abstract {
public:
    bool setup() override { return true; }
    bool shutdown() override { return true; }

    void setup_frame(std::shared_ptr<reflection::component::abstract_frame> frame);
    void add_log(const std::string& message);

private:
    std::vector<std::string> m_logs {};
    std::shared_ptr<reflection::component::abstract_list> m_console_list = nullptr;
};

#endif // __CONSOLE_MANAGER_HPP__