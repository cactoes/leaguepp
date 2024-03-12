#include <iostream>

#include "browser/browser.hpp"
#include "interface/holder.hpp"

#include "managers/layoutmanager.hpp"
#include "managers/browsermanager.hpp"
#include "managers/eventmanager.hpp"
#include "managers/connectormanager.hpp"
#include "managers/featuremanager.hpp"
#include "managers/configmanager.hpp"

int main() {
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
    interface::GetHolder<connector_manager>(from_singleton)->Setup();

    // start the browser
    browserManager->Start();

    return 0;
}