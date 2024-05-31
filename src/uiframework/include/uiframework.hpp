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

#define BROWSER_FLAG_DEV_TOOLS 0ul

struct BROWSER_CONFIG {
    BitField<unsigned long> flags = {};
};

// ~~ ISystemWindow base class
//    holds & manages all window & browser data

typedef CallbackContainer<void> FnOnContextMenuItemClicked;

class ISystemWindow {
public:
    virtual ~ISystemWindow(void) = default;
    virtual std::shared_ptr<component::IFrame> GetWindowFrame(void) = 0;
    virtual void CloseWindow(void) = 0;
    virtual void AddContextMenuItem(const std::string& group, const std::string& name, const FnOnContextMenuItemClicked& callback) = 0;
    virtual void CreateContextMenuGroup(const std::string& name) = 0;
    virtual void EnableContextMenu(bool state) = 0;
};

// call at the end of everything to start polling window events
void SystemPollWindowEvents(void);

// create a new window based on the config
std::shared_ptr<ISystemWindow> CreateSystemWindow(const WINDOW_CONFIG& wc, const BROWSER_CONFIG& bc, const component::LAYOUT& layout, const component::ALIGN& align = component::ALIGN::NONE);

enum class message_box_type {
    MB_ERROR = 0,
    MB_WARN,
    MB_INFO
};

// create a new message box window
std::shared_ptr<ISystemWindow> CreateMessageBox(const std::string& title, const std::string& message, message_box_type type);

#endif // __UIFRAMEWORK_HPP__