#ifndef __FRAME_HPP__
#define __FRAME_HPP__

#include <string>
#include <vector>
#include <memory>

#include "component.hpp"

#define FRAME_TARGET_MAIN "main"

namespace ui {
    struct frame_layout {
        enum values {
            HORIZONTAL = 0,
            VERTICAL
        };

        frame_layout(values val) : value(val) {}

        operator std::string() const;

        values value;
    };

    constexpr auto FL_HORIZONTAL = frame_layout::HORIZONTAL;
    constexpr auto FL_VERTICAL = frame_layout::VERTICAL;

    class frame : public component {
    public:
        frame(const std::string& name, const frame_layout& layout, const std::string& target = "") :
            m_name(name), m_layout(layout) {
            m_target = target;
        }

        component_type GetType() const override;
        void Register(browser::browser* handle) override;

        template <typename Ty, typename... Args>
        std::enable_if_t<std::is_base_of_v<component, Ty> | std::is_constructible_v<Ty, Args..., std::string>, void>
        AddComponent(Args&&... args) {
            m_children.push_back(std::make_unique<Ty>(std::forward<Args>(args)..., GetId()));
        }

        template <typename Ty>
        std::enable_if_t<std::is_base_of_v<component, Ty>, void>
        AddComponent(std::unique_ptr<Ty> comp) {
            comp->m_target = GetId();
            m_children.push_back(std::move(comp));
        }

    private:
        std::vector<std::unique_ptr<component>> m_children = {};

        std::string m_name;
        frame_layout m_layout;
    };
}; // namespace ui

#endif // __FRAME_HPP__