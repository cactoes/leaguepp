#include <iostream>

#include "uiframework.hpp"

#include <Windows.h>

int main(int, char**) {
    WINDOW_CONFIG config{};
    config.flags = { WINDOW_FLAG_SHOW, WINDOW_FLAG_HIDE_TITLE_BAR };
    config.size = { 200, 150 };
    config.name = "critical error";
    config.icon = 1;

    BROWSER_CONFIG bwConfig{};
    bwConfig.flags = { BROWSER_WINDOW_FLAG_DEV_TOOLS };

    auto systemWindow = CreateSystemWindow(config, bwConfig, component::LAYOUT::VERTICAL);
    auto frameMain = systemWindow->GetWindowFrame();

    auto parent = frameMain->AddFrame("", false, component::LAYOUT::HORIZONTAL);

    parent
        ->AddFrame("", false, component::LAYOUT::HORIZONTAL_AUTO, component::ALIGN::HORIZONTAL)
        ->AddImage("media/error_icon.png", { 50, 50 });

    parent
        ->AddFrame("", false, component::LAYOUT::VERTICAL, component::ALIGN::CENTER)
        ->AddLabel("Error message");

    auto b = frameMain->AddFrame("", false, component::LAYOUT::HORIZONTAL, component::ALIGN::CENTER);
    b->AddButton("ok", []() {
        std::cout << "clicked\n";
    });
    // b->AddButton("OK", []() {
    //     std::cout << "clicked\n";
    // });

    SystemPollWindowEvents();

    return 0;
}

// #include "service.hpp"

// class ServiceManager {
// public:
//     bool Init(void) {
//         m_manager = OpenSCManagerA(nullptr, SERVICES_ACTIVE_DATABASEA, SC_MANAGER_ALL_ACCESS);
//         if (m_manager == nullptr)
//             return false;

//         m_service = OpenServiceA(m_manager, SERVICE_NAME, SC_MANAGER_ALL_ACCESS);
//         if (m_service == nullptr) {
//             m_service = CreateLeagueService();
//             if (m_service == nullptr) {
//                 return false;
//             }
//         }

//         SERVICE_STATUS status{};
//         QueryServiceStatus(m_service, &status);
//         if (status.dwCurrentState == SERVICE_STOPPED)
//             return StartServiceA(m_service, 0, nullptr);

//         return status.dwCurrentState == SERVICE_RUNNING
//             || status.dwCurrentState == SERVICE_START_PENDING;
//     }

//     void Shutdown(void) {
//         if (m_manager) {
//             CloseServiceHandle(m_manager);
//             m_manager = nullptr;
//         }

//         if (m_service) {
//             CloseServiceHandle(m_service);
//             m_service = nullptr;
//         }
//     }

//     unsigned long QueryState(void) {
//         SERVICE_STATUS status{};
//         QueryServiceStatus(m_service, &status);
//         return status.dwCurrentState;
//     }

//     bool SendControlCode(unsigned long code) {
//         SERVICE_STATUS status{};
//         return ControlService(m_service, code, &status);
//     }

//     ~ServiceManager(void) {
//         Shutdown();
//     }

// private:
//     SC_HANDLE CreateLeagueService() {
//         char target[MAX_PATH] = {};
//         GetFullPathNameA("lppservice.exe", MAX_PATH, target, nullptr);

//         return CreateServiceA(m_manager,
//             SERVICE_NAME,
//             SERVICE_NAME_DETAIL,
//             SC_MANAGER_ALL_ACCESS,
//             SERVICE_WIN32_OWN_PROCESS,
//             SERVICE_DEMAND_START,
//             SERVICE_ERROR_NORMAL,
//             target,
//             nullptr, nullptr, nullptr, nullptr, nullptr);
//     }

// private:
//     SC_HANDLE m_manager = nullptr;
//     SC_HANDLE m_service = nullptr;
// };

// class ServiceIOClient : public IServiceIO {
// public:
//     ServiceIOClient(HANDLE handle) : IServiceIO(handle) {}

//     static ServiceIOClient Create(void) {
//         HANDLE handle = CreateFileA(SERVICE_PIPE_NAME,
//             GENERIC_READ | GENERIC_WRITE,
//             0,
//             nullptr,
//             OPEN_EXISTING,
//             0,
//             nullptr);

//         return ServiceIOClient(handle);
//     }
// };

    // auto serviceManager = ServiceManager();

    // if (!serviceManager.Init())
    //     return false;

    // while (serviceManager.QueryState() != SERVICE_RUNNING)
    //     Sleep(100);

    // auto client = ServiceIOClient::Create();

    // client.SendPacket(1234, 10);

    // serviceManager.SendControlCode(SERVICE_CONTROL_STOP);
