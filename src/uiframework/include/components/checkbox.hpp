#pragma once

#ifndef __COMPONENT_CHECKBOX_HPP__
#define __COMPONENT_CHECKBOX_HPP__

#include <vector>

#include "../component.hpp"

// ~~ checkbox class

namespace component {
    class ICheckbox : public IComponent {
    public:
        virtual ~ICheckbox(void) = default;

        virtual bool GetState(void) const = 0;
    };
}; // namespace component

#endif // __COMPONENT_CHECKBOX_HPP__