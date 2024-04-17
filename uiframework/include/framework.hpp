#ifndef __FRAMEWORK_HPP__
#define __FRAMEWORK_HPP__

#include <memory>
#include <string>

#include "frame.hpp"

#ifdef UIFRAMEWORK_EXPORTS
    #define UIFRAMEWORK_API __declspec(dllexport) [[nodiscard]]
#else
    #define UIFRAMEWORK_API __declspec(dllimport) [[nodiscard]]
#endif


class IUiFramework {
public:
    virtual bool Init() = 0;
    virtual void Shutdown() = 0;

    virtual void Run() = 0;

    virtual void CreateNotification(const std::string& title, const std::string& description) = 0;

    virtual std::shared_ptr<ui::Frame> GetMainFrame() = 0;
};

UIFRAMEWORK_API std::unique_ptr<IUiFramework> CreateFrameworkApi();

#endif // __FRAMEWORK_HPP__