#pragma once

#ifndef __COMPONENT_FRAME_HPP__
#define __COMPONENT_FRAME_HPP__

#include <vector>

#include "../component.hpp"

#include "label.hpp"
#include "button.hpp"
#include "image.hpp"
#include "folder_selector.hpp"

// ~~ frame class

namespace component {
    
    // struct for defining what layout the children should follow
    // ex. up->down / left->right
    struct LAYOUT {
        enum types {
            HORIZONTAL = 0,
            HORIZONTAL_AUTO,
            VERTICAL,
            VERTICAL_AUTO,
        };

        std::string ToString() const {
            switch (type) {
                case HORIZONTAL:       return "h";
                case HORIZONTAL_AUTO:  return "ha";
                case VERTICAL:         return "v";
                case VERTICAL_AUTO:    return "va";
                default:               return "";
            }
        }

        LAYOUT(types t) : type(t) {}
        types type;
    };

    // struct for defining what aligment the children should follow
    // ex. all on same horizontal line
    struct ALIGN {
        enum atypes {
            NONE = 0,
            HORIZONTAL,
            VERTICAL,
            CENTER,
        };

        std::string ToString() const {
            switch (type) {
                case HORIZONTAL:  return "ah";
                case VERTICAL:    return "av";
                case CENTER:      return "ac";
                default:          return "";
            }
        }

        ALIGN(atypes t) : type(t) {}
        atypes type;
    };

    class IFrame : public IComponent {
    public:
        virtual ~IFrame(void) = default;

        virtual void AddComponent(std::shared_ptr<IComponent> component) = 0;

        template <typename Ty>
        std::enable_if_t<std::is_base_of_v<IComponent, Ty>, std::shared_ptr<Ty>>
        GetComponent(const std::string& id) {
            for (auto& comp : m_components)
                if (comp->GetId() == id)
                    return std::static_pointer_cast<Ty>(comp);

            return nullptr;
        }

        virtual std::shared_ptr<IFrame> AddFrame(const std::string& name, bool hasBorder, const LAYOUT& layout, const ALIGN& align = component::ALIGN::NONE) = 0;
        virtual std::shared_ptr<ILabel> AddLabel(const std::string& name) = 0;
        virtual std::shared_ptr<IButton> AddButton(const std::string& name, const FnButtonCallback& callback) = 0;
        virtual std::shared_ptr<IImage> AddImage(const std::string& name, const VECTOR2I& size) = 0;
        virtual std::shared_ptr<IFolderSelector> AddFolderSelector(const std::string& name) = 0;

    protected:
        std::vector<std::shared_ptr<IComponent>> m_components = {};
    };
}; // namespace component

#endif // __COMPONENT_FRAME_HPP__