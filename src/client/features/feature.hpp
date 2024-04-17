#ifndef __FEATURE_HPP__
#define __FEATURE_HPP__

#include <memory>
#include <string>
#include <framework.hpp>

namespace feature {
    class Feature {
    public:
        virtual void Setup(std::shared_ptr<ui::Frame> frame, IUiFramework* frameworkApiHandle) = 0;
        virtual std::string GetName() = 0;
    };
}; // namespace feature

#endif // __FEATURE_HPP__