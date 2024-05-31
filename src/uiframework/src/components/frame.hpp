#pragma once

#ifndef __IMPL_FRAME_HPP__
#define __IMPL_FRAME_HPP__

#include "components/frame.hpp"
#include "components/label.hpp"
#include "components/button.hpp"
#include "components/image.hpp"
#include "components/folder_selector.hpp"
#include "browser.hpp"

namespace component {
    class Frame : public IFrame {
    public:
        Frame(const std::string& name, Browser* browser, const std::string& target, bool hasBorder, const LAYOUT& layout, const ALIGN& align = ALIGN::NONE) 
            : m_hasBorder(hasBorder), m_layout(layout), m_align(align) {
            m_name = name;
            m_browser = browser;
            m_target = target;
        }

        void Update(void) override {}

        void Render(void) override {
            m_browser->CallJSFunction("createUIComponent", { "frame", m_name, GetId(), m_target, m_hasBorder, m_layout.ToString(), m_align.ToString() });

            for (const auto& comp : m_components)
                comp->Render();
        }

        template <typename Ty, typename... Args>
        std::enable_if_t<std::is_base_of_v<IComponent, Ty> | std::is_constructible_v<Ty, Args...>, std::shared_ptr<Ty>>
        AddComponent(const std::string& name, Args&&... args) {
            auto ptr = std::make_shared<Ty>(name, m_browser, GetId(), std::forward<Args>(args)...);
            AddComponent(ptr);
            return ptr;
        }

        void AddComponent(std::shared_ptr<IComponent> component) override {
            component->SetTarget(GetId());
            m_components.push_back(std::move(component));
        }

        std::shared_ptr<IFrame> AddFrame(const std::string& name, bool hasBorder, const LAYOUT& layout, const ALIGN& align = ALIGN::NONE) override {
            return AddComponent<Frame>(name, hasBorder, layout, align);
        }

        std::shared_ptr<ILabel> AddLabel(const std::string& name) override {
            return AddComponent<Label>(name);
        }

        std::shared_ptr<IButton> AddButton(const std::string& name, const FnButtonCallback& callback) override {
            return AddComponent<Button>(name, callback);
        }

        std::shared_ptr<IImage> AddImage(const std::string& name, const VECTOR2I& size) override {
            return AddComponent<Image>(name, size);
        }

        std::shared_ptr<IFolderSelector> AddFolderSelector(const std::string& name) override {
            return AddComponent<FolderSelector>(name);
        }

    private:
        Browser* m_browser;
        bool m_hasBorder;
        LAYOUT m_layout;
        ALIGN m_align;
    };
}; // namespace component

#endif // __IMPL_FRAME_HPP__