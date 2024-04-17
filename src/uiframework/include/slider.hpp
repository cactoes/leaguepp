#ifndef __COMPONENT_SLIDER_HPP__
#define __COMPONENT_SLIDER_HPP__

#include "component.hpp"

namespace ui {
    // void SliderCallback(int newValue)
    typedef callback_holder<void, int> slider_callback;

    class Slider : public Component {
    public:
        virtual ~Slider() = default;
    };
}; // namespace ui

#endif // __COMPONENT_SLIDER_HPP__