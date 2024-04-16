#include "interface/holder.hpp"

#include "managers/layoutmanager.hpp"
#include "managers/browsermanager.hpp"
#include "managers/connectormanager.hpp"
#include "managers/featuremanager.hpp"
#include "managers/configmanager.hpp"

#include "utils.hpp"

#undef interface

int main(UNUSED int argc, UNUSED char** argv) {
    auto browserManager = interface<BrowserManager>::Get();
    browserManager->Init();

    auto connectorManager = interface<ConnectorManager>::Get();
    connectorManager->Init();

    interface<ConfigManager>::Get()->Init();
    
    interface<LayoutManager>::Get()->Init();
    interface<FeatureManager>::Get()->Init();

    browserManager->Start();
    connectorManager->Shutdown();

    return 0;
}