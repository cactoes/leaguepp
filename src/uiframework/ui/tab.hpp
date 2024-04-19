#ifndef __TAB_HPP__
#define __TAB_HPP__

#include "../include/tab.hpp"
#include "../browser/browser.hpp"
#include "frame.hpp"

namespace ui {
    class TabImpl : public Tab {
    public:
        TabImpl(const std::string& label, bool isActive, browser::Browser* handle, const std::string& target) :
            m_label(label), m_isActive(isActive) {
            m_handle = handle;
            m_target = target;

            m_frame = std::make_shared<ui::FrameImpl>("", FL_HORIZONTAL, m_handle, GetId());
        }

        component_type GetType() const override;
        void Update() override;
        void Register() override;

        std::shared_ptr<ui::Frame> GetFrame() override;
        void SetActive() override;

    private:
        bool m_isActive;
        std::string m_label;
        std::shared_ptr<ui::Frame> m_frame;
    };
}; // namespace ui

#endif // __TAB_HPP__