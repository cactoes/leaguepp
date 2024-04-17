#ifndef __SLIDER_HPP__
#define __SLIDER_HPP__

#include "../include/slider.hpp"
#include "../browser/browser.hpp"

namespace ui {
    class SliderImpl : public Slider {
    public:
        SliderImpl(const std::string& label, int min, int max, const slider_callback& callbackHolder, browser::Browser* handle, const std::string& target = "") :
            m_label(label), m_min(min), m_max(max), m_callbackHolder(callbackHolder) {
            m_target = target;
            m_handle = handle;
        }

        component_type GetType() const override;
        void Update() override;
        void Register() override;

        void HandleChange(browser::Browser*, browser::js_args_t args);

    private:
        std::string m_label;
        int m_min;
        int m_max;
        slider_callback m_callbackHolder;
    };
}; // namespace ui

#endif // __SLIDER_HPP__