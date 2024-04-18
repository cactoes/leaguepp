#include "interface/holder.hpp"

#include <framework.hpp>

#include "managers/connectormanager.hpp"
#include "managers/featuremanager.hpp"
#include "managers/configmanager.hpp"
#include "managers/resourcemanager.hpp"

#include <utils.hpp>

#undef interface

int main(UNUSED int argc, UNUSED char** argv) {
    std::unique_ptr<IUiFramework> frameworkApiHandle = CreateFrameworkApi();

    frameworkApiHandle->Init();

    auto connectorManager = interface<ConnectorManager>::Get();
    connectorManager->Init();

    interface<ResourceManager>::Get()->Init();
    interface<ConfigManager>::Get()->Init();
    interface<FeatureManager>::Get()->Init(frameworkApiHandle.get());

    frameworkApiHandle->Run();
    connectorManager->Shutdown();

    return 0;
}