#ifndef __COMPONENT_BUTTON_HPP__
#define __COMPONENT_BUTTON_HPP__

#include "component.hpp"

namespace ui {
    // void ButtonCallback(void)
    typedef callback_holder<void> button_callback;

    class Button : public Component {
    public:
    };
}; // namespace ui

#endif // __COMPONENT_BUTTON_HPP__