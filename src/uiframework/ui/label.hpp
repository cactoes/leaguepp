#ifndef __LABEL_HPP__
#define __LABEL_HPP__

#include "../include/label.hpp"
#include "../browser/browser.hpp"

namespace ui {
    class LabelImpl : public Label {
    public:
        LabelImpl(const std::string& text, const std::string& pos, browser::Browser* handle, const std::string& target = "") :
            m_text(text), m_pos(pos) {
            m_target = target;
            m_handle = handle;
        }

        component_type GetType() const override;
        void Update() override;
        void Register() override;

        void SetText(const std::string& text) override;

    private:
        std::string m_text;
        std::string m_pos;
    };
}; // namespace ui

#endif // __LABEL_HPP__