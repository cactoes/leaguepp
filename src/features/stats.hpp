#ifndef __STATS_HPP__
#define __STATS_HPP__

#include "feature.hpp"

namespace feature {
    class Stats : public Feature {
    public:
        void Setup(std::shared_ptr<ui::Frame> frame) override;
        std::string GetName() override;
    };
}; // namespace feature

#endif // __STATS_HPP__