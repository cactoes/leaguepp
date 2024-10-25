#pragma once

#ifndef __MANAGER_HPP__
#define __MANAGER_HPP__

#include <memory>
#include <type_traits>

namespace manager {
    struct create_t {};
    inline constexpr create_t create {};

    class abstract {
    public:
        ~abstract() = default;

        virtual bool setup() = 0;
        virtual bool shutdown() = 0;
    };
    
    template <typename _data_type>
    inline std::enable_if_t<std::is_base_of_v<abstract, _data_type>, std::shared_ptr<_data_type>> instance() {
        const static std::shared_ptr<_data_type> _instance = std::make_shared<_data_type>();
        return _instance;
    }

    template <typename _data_type>
    inline std::enable_if_t<std::is_base_of_v<abstract, _data_type>, std::unique_ptr<_data_type>> instance(create_t) {
        return std::make_unique<_data_type>();
    }
} // namespace manager

#endif // __MANAGER_HPP__