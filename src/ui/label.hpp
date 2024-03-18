#ifndef __LABEL_HPP__
#define __LABEL_HPP__

#include <string>

#include "component.hpp"

namespace ui {
    class label : public component {
    public:
        label(const std::string& text, const std::string& pos, const std::string& target = "") :
            m_text(text), m_pos(pos) {
            m_target = target;
        }

        component_type GetType() const override;
        void Register(browser::browser* handle) override;
        void Update(browser::browser* handle) override;

        void SetText(const std::string& text);

    private:
        std::string m_text;
        std::string m_pos;
    };
}; // namespace ui

#endif // __LABEL_HPP__