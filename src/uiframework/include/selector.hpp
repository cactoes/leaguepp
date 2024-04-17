#ifndef __COMPONENT_SELECTOR_HPP__
#define __COMPONENT_SELECTOR_HPP__

#include "component.hpp"

namespace ui {
    // int SelectorCallback(std::string currentItem)
    typedef callback_holder<int, std::string> selector_callback;

    class Selector : public Component {
    public:
        virtual ~Selector() = default;
    };
}; // namespace ui

#endif // __COMPONENT_SELECTOR_HPP__