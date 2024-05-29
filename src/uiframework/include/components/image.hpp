#pragma once

#ifndef __COMPONENT_IMAGE_HPP__
#define __COMPONENT_IMAGE_HPP__

#include <vector>

#include "../component.hpp"

// ~~ image class

namespace component {
    class IImage : public IComponent {
    public:
        virtual ~IImage(void) = default;
    };
}; // namespace component

#endif // __COMPONENT_IMAGE_HPP__