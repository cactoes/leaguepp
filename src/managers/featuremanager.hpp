#ifndef __FEATUREMANAGER_HPP__
#define __FEATUREMANAGER_HPP__

#include <map>

#include "../features/feature.hpp"
#include "../hash.hpp"

class feature_manager {
public:
    void Setup();

    template <typename Ty>
    std::enable_if_t<std::is_base_of_v<feature::feature, Ty>, void>
    CreateFeature(std::shared_ptr<ui::frame> frame) {
        auto feature = std::make_unique<Ty>();
        feature->Setup(std::move(frame));
        m_features[hash::Fnv1a64Hash(feature->GetName().c_str())] = std::move(feature);
    }

private:
    std::map<uint64_t, std::unique_ptr<feature::feature>> m_features;
};

#endif // __FEATUREMANAGER_HPP__