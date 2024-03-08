#ifndef __LABEL_HPP__
#define __LABEL_HPP__

#include <string>

#include "component.hpp"

namespace ui {
    class label : public component {
    public:
        label(const std::string& text, const std::string& target = "") :
            m_text(text) {
            m_target = target;
        }

        component_type GetType() const override;
        void Register(browser::browser* handle) override;

    private:
        std::string m_text;
    };
}; // namespace ui

#endif // __LABEL_HPP__