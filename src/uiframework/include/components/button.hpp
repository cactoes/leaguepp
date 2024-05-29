#pragma once

#ifndef __COMPONENT_BUTTON_HPP__
#define __COMPONENT_BUTTON_HPP__

#include <vector>

#include "../component.hpp"

// ~~ button class

namespace component {
    typedef CallbackContainer<void> FnButtonCallback;

    class IButton : public IComponent {
    public:
        virtual ~IButton(void) = default;
    };
}; // namespace component

#endif // __COMPONENT_BUTTON_HPP__