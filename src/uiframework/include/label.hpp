#ifndef __COMPONENT_LABEL_HPP__
#define __COMPONENT_LABEL_HPP__

#include "component.hpp"

namespace ui {
    class Label : public Component {
    public:
        virtual ~Label() = default;

        virtual void SetText(const std::string& text) = 0;
    };
}; // namespace ui

#endif // __COMPONENT_LABEL_HPP__