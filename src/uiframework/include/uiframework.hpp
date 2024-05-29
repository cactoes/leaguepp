#pragma once

#ifndef __UIFRAMEWORK_HPP__
#define __UIFRAMEWORK_HPP__

#include <string>
#include <memory>
#include <bitfield.hpp>

#include "components/frame.hpp"

// ~~ ui framework api header

#define WINDOW_FLAG_RESIZEABLE 0ul
#define WINDOW_FLAG_HIDE_TITLE_BAR 1ul
#define WINDOW_FLAG_SHOW 2ul

// RESIZEABLE w/ HIDE_TITLE_BAR might not give proper behaviour
struct WINDOW_CONFIG {
    VECTOR2I size = { 700, 600 };
    VECTOR2I pos = { 0, 0 };
    int icon = -1;
    std::string name = "window";
    BitField<unsigned long> flags = {};
};

#define BROWSER_WINDOW_FLAG_DEV_TOOLS 0ul

struct BROWSER_CONFIG {
    BitField<unsigned long> flags = {};
};

// ~~ ISystemWindow base class
//    holds & manages all window & browser data

class ISystemWindow {
public:
    virtual ~ISystemWindow(void) = default;
    virtual std::shared_ptr<component::IFrame> GetWindowFrame(void) = 0;
};

// call at the end of everything to start polling window events
void SystemPollWindowEvents(void);

// create a new window based on the config
std::unique_ptr<ISystemWindow> CreateSystemWindow(const WINDOW_CONFIG& wc, const BROWSER_CONFIG& bc, const component::LAYOUT& layout, const component::ALIGN& align = component::ALIGN::NONE);

#endif // __UIFRAMEWORK_HPP__