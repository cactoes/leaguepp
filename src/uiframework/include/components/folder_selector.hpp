#pragma once

#ifndef __COMPONENT_FOLDER_SELECTOR_HPP__
#define __COMPONENT_FOLDER_SELECTOR_HPP__

#include <vector>

#include "../component.hpp"

// ~~ FolderSelector class
//    opens a dialogue to select a folder

namespace component {
    class IFolderSelector : public IComponent {
    public:
        virtual ~IFolderSelector(void) = default;

        virtual const std::string& GetPath(void) const = 0;
    };
}; // namespace component

#endif // __COMPONENT_FOLDER_SELECTOR_HPP__