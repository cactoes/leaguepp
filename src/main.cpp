#include "interface/holder.hpp"

#include "managers/layoutmanager.hpp"
#include "managers/browsermanager.hpp"
#include "managers/connectormanager.hpp"
#include "managers/featuremanager.hpp"
#include "managers/configmanager.hpp"

#include "utils.hpp"

#undef interface

int main(UNUSED int argc, UNUSED char** argv) {
    auto browserManager = interface<browser_manager>::Get();
    browserManager->Setup();

    interface<ConfigManager>::Get()->Init();
    interface<LayoutManager>::Get()->Init();
    interface<FeatureManager>::Get()->Init();

    auto connectionManager = interface<ConnectorManager>::Get();
    connectionManager->Init();

    browserManager->Start();
    connectionManager->Shutdown();

    return 0;
}