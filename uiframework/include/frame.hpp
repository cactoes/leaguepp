#ifndef __COMPONENT_FRAME_HPP__
#define __COMPONENT_FRAME_HPP__

#include "component.hpp"

#include "button.hpp"
#include "checkbox.hpp"
#include "dropdown.hpp"
#include "input.hpp"
#include "label.hpp"
#include "selector.hpp"
#include "slider.hpp"

#define FRAME_TARGET_MAIN "main"

namespace ui {
    struct frame_layout {
        enum values {
            HORIZONTAL = 0,
            HORIZONTAL_AUTO,
            VERTICAL,
            VERTICAL_AUTO,
        };

        frame_layout(values val) : value(val) {}

        operator std::string() const {
            switch (value) {
                case HORIZONTAL:
                    return "horizontal";
                case HORIZONTAL_AUTO:
                    return "horizontal-auto";
                case VERTICAL:
                    return "vertical";
                case VERTICAL_AUTO:
                    return "vertical-auto";
                default:
                    return "";
            }
        }

        values value;
    };

    constexpr auto FL_HORIZONTAL = frame_layout::HORIZONTAL;
    constexpr auto FL_HORIZONTAL_AUTO = frame_layout::HORIZONTAL_AUTO;
    constexpr auto FL_VERTICAL = frame_layout::VERTICAL;
    constexpr auto FL_VERTICAL_AUTO = frame_layout::VERTICAL_AUTO;

    class Frame : public Component {
    public:
        virtual std::shared_ptr<Button> AddButton(const std::string& label, const button_callback& callbackHolder) = 0;
        virtual std::shared_ptr<Checkbox> AddCheckbox(const std::string& label, bool state, const checkbox_callback& callbackHolder) = 0;
        virtual std::shared_ptr<Dropdown> AddDropdown(const std::string& label, bool isMulti, const std::vector<std::string>& activeValues, const std::vector<std::string>& values, const dropdown_callback& callbackHolder) = 0;
        virtual std::shared_ptr<Input> AddInput(const std::string& label, const std::string& value, const input_callback& callbackHolder) = 0;
        virtual std::shared_ptr<Label> AddLabel(const std::string& text, const std::string& pos) = 0;
        virtual std::shared_ptr<Selector> AddSelector(const std::string& label, int state, const std::vector<std::string>& values, const selector_callback& callbackHolder) = 0;
        virtual std::shared_ptr<Slider> AddSlider(const std::string& label, int min, int max, const slider_callback& callbackHolder) = 0;
        virtual std::shared_ptr<Frame> AddFrame(const std::string& name, const frame_layout& layout) = 0;

        virtual void AddComponent(std::shared_ptr<Component> component) = 0;

        template <typename Ty>
        std::enable_if_t<std::is_base_of_v<Component, Ty>, std::shared_ptr<Ty>>
        GetComponent(const std::string& id) {
            for (auto& comp : m_components)
                if (comp->GetId() == id)
                    return std::static_pointer_cast<Ty>(comp);

            return nullptr;
        }

    protected:
        std::vector<std::shared_ptr<Component>> m_components = {};
    };
}; // namespace ui

#endif // __COMPONENT_FRAME_HPP__