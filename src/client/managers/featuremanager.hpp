#ifndef __FEATUREMANAGER_HPP__
#define __FEATUREMANAGER_HPP__

#include <map>
#include <framework.hpp>
#include <hash.hpp>

#include "../features/feature.hpp"

class FeatureManager {
public:
    bool Init(IUiFramework* frameworkApiHandle);

    template <typename Ty>
    std::enable_if_t<std::is_base_of_v<feature::Feature, Ty>, void>
    CreateFeature(std::shared_ptr<ui::Frame> parent, IUiFramework* frameworkApiHandle) {
        auto feature = std::make_unique<Ty>();
        feature->Setup(std::move(parent), frameworkApiHandle);
        m_features[hash::Fnv1a64Hash(feature->GetName().c_str())] = std::move(feature);
    }

private:
    std::map<uint64_t, std::unique_ptr<feature::Feature>> m_features;
};

#endif // __FEATUREMANAGER_HPP__