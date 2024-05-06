#include "interface/holder.hpp"

#include <framework.hpp>

#include "managers/connectormanager.hpp"
#include "managers/featuremanager.hpp"
#include "managers/configmanager.hpp"
#include "managers/resourcemanager.hpp"
#include "managers/proxymanager.hpp"

#include <utils.hpp>

#undef interface

int main(UNUSED int argc, UNUSED char** argv) {
    std::unique_ptr<IUiFramework> frameworkApiHandle = CreateFrameworkApi();

    frameworkApiHandle->Init();

    interface<ResourceManager>::Get()->Init();
    interface<ConfigManager>::Get()->Init();
    interface<FeatureManager>::Get()->Init(frameworkApiHandle.get());

    auto proxyManager = interface<ProxyManager>::Get();
    proxyManager->Init(frameworkApiHandle.get());

    auto connectorManager = interface<ConnectorManager>::Get();
    connectorManager->Init();

    frameworkApiHandle->Run();

    connectorManager->Shutdown();

    proxyManager->Shutdown();

    return 0;
}