#ifndef __SLIDER_HPP__
#define __SLIDER_HPP__

#include <string>

#include "component.hpp"

namespace ui {
    typedef callback_holder<void, int> slider_callback_t;

    class slider : public component {
    public:
        slider(const std::string& label, int min, int max, const slider_callback_t& callbackHolder, const std::string& target = "") :
            m_label(label), m_min(min), m_max(max), m_callbackHolder(callbackHolder) {
            m_target = target;
        }

        component_type GetType() const override;
        void Register(browser::browser* handle) override;
        void Update(browser::browser* handle) override;

        void HandleChange(browser::browser*, browser::js_args_t args);

    private:
        std::string m_label;
        int m_min;
        int m_max;
        slider_callback_t m_callbackHolder;
    };
}; // namespace ui

#endif // __SLIDER_HPP__