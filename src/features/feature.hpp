#ifndef __FEATURE_HPP__
#define __FEATURE_HPP__

#include <memory>
#include <string>

#include "../ui/component.hpp"
#include "../ui/frame.hpp"

namespace feature {
    class feature {
    public:
        virtual void Setup(std::shared_ptr<ui::frame> frame) = 0;
        virtual std::string GetName() = 0;
    };
}; // namespace feature

#endif // __FEATURE_HPP__