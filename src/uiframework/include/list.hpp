#ifndef __COMPONENT_LIST_HPP__
#define __COMPONENT_LIST_HPP__

#include "component.hpp"

namespace ui {
    // bool validator(std::string itemToAdd)
    typedef callback_holder<bool, std::string> list_validator_callback;

    // std::vector<std::string> ListCallback(std::vector<std::string> list)
    typedef callback_holder<std::vector<std::string>, std::vector<std::string>> list_callback;

    class List : public Component {
    public:
        virtual ~List() = default;

        virtual void SetActiveItems(const std::vector<std::string>& items) = 0;
    };
}; // namespace ui

#endif // __COMPONENT_LIST_HPP__