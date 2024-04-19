#ifndef __COMPONENT_TAB_HPP__
#define __COMPONENT_TAB_HPP__

#include "component.hpp"
#include "frame.hpp"

namespace ui {
    class Tab : public Component {
    public:
        virtual ~Tab() = default;

        virtual std::shared_ptr<ui::Frame> GetFrame() = 0;
        virtual void SetActive() = 0;
    };
}; // namespace ui

#endif // __COMPONENT_TAB_HPP__