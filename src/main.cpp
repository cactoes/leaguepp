#include <iostream>

#include "browser/browser.hpp"
#include "interface/holder.hpp"

#include "managers/layoutmanager.hpp"
#include "managers/browsermanager.hpp"
#include "managers/eventmanager.hpp"
#include "managers/connectormanager.hpp"
#include "managers/featuremanager.hpp"
#include "managers/configmanager.hpp"

int main(int argc, char** argv) {
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    // setup the browser
    auto browserManager = interface::GetHolder<browser_manager>(from_singleton);
    browserManager->Setup();

    // load user settings
    interface::GetHolder<config_manager>(from_singleton)->Setup();

    // create frames
    interface::GetHolder<layout_manager>(from_singleton)->Setup(browserManager->GetHandle());

    // setup features
    interface::GetHolder<feature_manager>(from_singleton)->Setup();

    // setup window events
    interface::GetHolder<event_manager>(from_singleton)->Setup(browserManager->GetHandle());

    // setup league connector & try to connect
    auto connectionManager = interface::GetHolder<connector_manager>(from_singleton);
    connectionManager->Setup();

    // start the browser
    browserManager->Start();

    // stop the connector
    connectionManager->Shutdown();

    return 0;
}