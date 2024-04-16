#ifndef __PROFILE_HPP__
#define __PROFILE_HPP__

#include "feature.hpp"

namespace feature {
    class Profile : public Feature {
    public:
        void Setup(std::shared_ptr<ui::Frame> frame) override;
        std::string GetName() override;
    };
}; // namespace feature

#endif // __PROFILE_HPP__