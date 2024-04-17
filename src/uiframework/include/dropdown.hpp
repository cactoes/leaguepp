#ifndef __COMPONENT_DROPDOWN_HPP__
#define __COMPONENT_DROPDOWN_HPP__

#include "component.hpp"

namespace ui {
    // void DropDownCallback(std::string item, bool newItemState, std::vector<std::string> list)
    typedef callback_holder<std::vector<std::string>, std::string, bool, std::vector<std::string>> dropdown_callback;

    class Dropdown : public Component {
    public:
        virtual ~Dropdown() = default;
    };
}; // namespace ui

#endif // __COMPONENT_DROPDOWN_HPP__