#ifndef __COMPONENT_INPUT_HPP__
#define __COMPONENT_INPUT_HPP__

#include "component.hpp"

namespace ui {
    // void ButtonCallback(void)
    typedef callback_holder<std::string, std::string> input_callback;

    class Input : public Component {
    public:
    };
}; // namespace ui

#endif // __COMPONENT_INPUT_HPP__