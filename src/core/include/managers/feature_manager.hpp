#pragma once

#ifndef __FEATURE_MANAGER_HPP__
#define __FEATURE_MANAGER_HPP__

#include <memory>
#include <map>
#include <vui/vui_core.hpp>

#include "manager.hpp"

class abstract_feature {
public:
    ~abstract_feature() = default;

    virtual void setup(std::shared_ptr<vui::component::abstract_frame> frame) = 0;
};

class feature_manager : public manager::abstract {
public:
    bool setup() override { return true; }
    bool shutdown() override { return true; }

    template <typename _feature_type>
    std::enable_if_t<std::is_base_of_v<abstract_feature, _feature_type>, void> add_feature(std::shared_ptr<vui::component::abstract_frame> frame) {
        std::unique_ptr<abstract_feature> feature = std::make_unique<_feature_type>();
        feature->setup(frame);
        m_features[typeid(_feature_type).hash_code()] = std::move(feature);
    }

private:
    std::map<size_t, std::unique_ptr<abstract_feature>> m_features = {};
};

#endif // __FEATURE_MANAGER_HPP__