#ifndef __COMPONENT_CHECKBOX_HPP__
#define __COMPONENT_CHECKBOX_HPP__

#include "component.hpp"

namespace ui {
    // bool CheckboxChange(bool newState)
    typedef callback_holder<bool, bool> checkbox_callback;

    class Checkbox : public Component {
    public:
        virtual ~Checkbox() = default;
    };
}; // namespace ui

#endif // __COMPONENT_CHECKBOX_HPP__