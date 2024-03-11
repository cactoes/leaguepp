#include <iostream>

#include "browser/browser.hpp"
#include "interface/holder.hpp"

#include "managers/layoutmanager.hpp"
#include "managers/browsermanager.hpp"
#include "managers/eventmanager.hpp"
#include "managers/connectormanager.hpp"

int main() {
    auto browserManager = interface::GetHolder<browser_manager>(from_singleton);
    browserManager->Setup();

    interface::GetHolder<layout_manager>(from_singleton)->Setup(browserManager->GetHandle());
    interface::GetHolder<event_manager>(from_singleton)->Setup(browserManager->GetHandle());
    interface::GetHolder<connector_manager>(from_singleton)->Setup();

    // blocking until window is closed
    browserManager->Start();

    return 0;
}