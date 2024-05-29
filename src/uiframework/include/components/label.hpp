#pragma once

#ifndef __COMPONENT_LABEL_HPP__
#define __COMPONENT_LABEL_HPP__

#include <vector>

#include "../component.hpp"

// ~~ label class

namespace component {
    class ILabel : public IComponent {
    public:
        virtual ~ILabel(void) = default;

        virtual void SetText(const std::string& text) = 0;
    };
}; // namespace component

#endif // __COMPONENT_LABEL_HPP__