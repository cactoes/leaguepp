#include <iostream>

#include "browser/browser.hpp"
#include "interface/holder.hpp"

#include "managers/layoutmanager.hpp"
#include "managers/browsermanager.hpp"
#include "managers/connectormanager.hpp"
#include "managers/featuremanager.hpp"
#include "managers/configmanager.hpp"

int main(int argc, char** argv) {
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    // setup the browser
    auto browserManager = interface<browser_manager>::Get();
    browserManager->Setup();

    // load user settings
    interface<config_manager>::Get()->Setup();

    // create frames
    interface<layout_manager>::Get()->Setup(browserManager->GetHandle());

    // setup features
    interface<feature_manager>::Get()->Setup();

    // setup league connector & try to connect
    auto connectionManager = interface<connector_manager>::Get();
    connectionManager->Setup();

    // start the browser
    browserManager->Start();

    // stop the connector
    connectionManager->Shutdown();

    return 0;
}