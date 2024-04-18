#ifndef __FRAME_HPP__
#define __FRAME_HPP__

#include <memory>

#include "../include/frame.hpp"
#include "../browser/browser.hpp"

namespace ui {
    class FrameImpl : public Frame {
    public:
        FrameImpl(const std::string& name, const frame_layout& layout, browser::Browser* handle, const std::string& target = "") :
            m_name(name), m_layout(layout) {
            m_target = target;
            m_handle = handle;
        }

        component_type GetType() const override;
        void Update() override;
        void Register() override;

        template <typename Ty, typename... Args>
        std::enable_if_t<std::is_base_of_v<Component, Ty> | std::is_constructible_v<Ty, Args..., std::string>, std::shared_ptr<Ty>>
        AddComponent(Args&&... args) {
            auto ptr = std::make_shared<Ty>(std::forward<Args>(args)..., GetId());
            m_components.push_back(ptr);
            return ptr;
        }

        template <typename Ty>
        std::enable_if_t<std::is_base_of_v<Component, Ty>, void>
        AddComponent(std::shared_ptr<Ty> comp) {
            comp->m_target = GetId();
            m_components.push_back(std::move(comp));
        }

        std::shared_ptr<Button> AddButton(const std::string& label, const button_callback& callbackHolder) override;
        std::shared_ptr<Checkbox> AddCheckbox(const std::string& label, const std::string& info, bool state, const checkbox_callback& callbackHolder) override;
        std::shared_ptr<Dropdown> AddDropdown(const std::string& label, bool isMulti, const std::vector<std::string>& activeValues, const std::vector<std::string>& values, const dropdown_callback& callbackHolder) override;
        std::shared_ptr<Input> AddInput(const std::string& label, const std::string& value, const input_callback& callbackHolder) override;
        std::shared_ptr<Label> AddLabel(const std::string& text, const std::string& pos) override;
        std::shared_ptr<Selector> AddSelector(const std::string& label, int state, const std::vector<std::string>& values, const selector_callback& callbackHolder) override;
        std::shared_ptr<Slider> AddSlider(const std::string& label, int min, int max, const slider_callback& callbackHolder) override;
        std::shared_ptr<List> AddList(const std::string& label, const std::vector<std::string>& activeValues, const list_validator_callback& validator, const list_callback& callbackHolder) override;
        std::shared_ptr<Frame> AddFrame(const std::string& name, const frame_layout& layout) override;

        void AddComponent(std::shared_ptr<Component> component) override;
        
    private:
        std::string m_name;
        frame_layout m_layout;
    };
}; // namespace ui

#endif // __FRAME_HPP__