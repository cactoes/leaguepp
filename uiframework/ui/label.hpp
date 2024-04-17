#ifndef __LABEL_HPP__
#define __LABEL_HPP__

#include "../include/label.hpp"
#include "../browser/browser.hpp"

namespace ui {
    class LabelImpl : public Label {
    public:
        LabelImpl(const std::string& text, const std::string& pos, const std::string& target = "") :
            m_text(text), m_pos(pos) {
            m_target = target;
        }

        component_type GetType() const override;
        void Update() override;
        void Register(browser::Browser* handle) override;

        void SetText(const std::string& text) override;

    private:
        browser::Browser* m_handle;
        std::string m_text;
        std::string m_pos;
    };
}; // namespace ui

#endif // __LABEL_HPP__