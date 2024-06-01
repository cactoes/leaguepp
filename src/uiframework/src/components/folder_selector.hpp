#pragma once

#ifndef __IMPL_FOLDER_SELECTOR_HPP__
#define __IMPL_FOLDER_SELECTOR_HPP__

#include "components/folder_selector.hpp"
#include "browser.hpp"

namespace component {
    class FolderSelector : public IFolderSelector {
    public:
        FolderSelector(const std::string& name, Browser* browser, const std::string& target) {
            m_name = name;
            m_browser = browser;
            m_target = target;
        }

        void Update(void) override {}

        void Render(void) override {
            m_browser->CallJSFunction("createUIComponent", { "folderSelector", m_name, GetId(), m_target, m_path, COMPONENT_CALLER_ID(OnSetPath) });
            m_browser->RegisterFunction(COMPONENT_CALLER_ID(OnSetPath), &FolderSelector::OnSetPath, this);
        }

        void OnSetPath(Browser*, JSArgs args) {
            m_path = args.at(0);
        }

        const std::string& GetPath(void) const override {
            return m_path;
        }

    private:
        Browser* m_browser;
        std::string m_path;
    };
}; // namespace component

#endif // __IMPL_FOLDER_SELECTOR_HPP__