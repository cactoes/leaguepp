#include <Windows.h>
#include <iostream>
#include <uiframework.hpp>
#include "service.hpp"

// template <typename handle, auto func>
// class FunctionWrapper {
// public:
//     template <typename... arguments>
//     FunctionWrapper(arguments&&... args)
//         requires std::invocable<decltype(func), arguments...>
//         : m_handle(func(std::forward<arguments>(args)...)) {};

//     FunctionWrapper(const FunctionWrapper&) = delete;
//     FunctionWrapper& operator=(const FunctionWrapper& rhs) = delete;

//     FunctionWrapper(FunctionWrapper&& r) {
//         m_handle = std::move(r.m_handle);
//         r.m_handle = nullptr;
//     }

//     FunctionWrapper& operator=(FunctionWrapper&& r) {
//         if (this != &r) {
//             this->~FunctionWrapper();
//             m_handle = std::move(r.m_handle);
//             r.m_handle = nullptr;
//         }
//         return *this;
//     };

//     handle* operator->() {
//         return &m_handle;
//     }

// public:
//     handle m_handle;
// };

// typedef FunctionWrapper<win32::sc_handle, &OpenSCManagerA> ServiceManager;

// class ServiceManager {
// public:
//     bool Init(void) {
//         m_manager = OpenSCManagerA(nullptr, SERVICES_ACTIVE_DATABASEA, SC_MANAGER_CONNECT);
//         if (!m_manager.IsValid()) {
//             std::cout << "open scma failed\n";
//             return false;
//         }

//         m_service = m_manager.Call(&OpenServiceA, SERVICE_NAME, SERVICE_START | SERVICE_STOP | SERVICE_QUERY_STATUS);
//         if (!m_service.IsValid()) {
//             // m_service = CreateLeagueService();
//             std::cout << "open service failed\n";
//             return false;
//         }

//         SERVICE_STATUS status{};
//         m_service.Call(&QueryServiceStatus, &status);
//         if (status.dwCurrentState == SERVICE_STOPPED)
//             return m_service.Call(&StartServiceA, 0, nullptr);

//         return status.dwCurrentState == SERVICE_RUNNING
//             || status.dwCurrentState == SERVICE_START_PENDING;
//     }

//     unsigned long QueryState(void) {
//         SERVICE_STATUS status{};
//         m_service.Call(&QueryServiceStatus, &status);

//         return status.dwCurrentState;
//     }

//     bool SendControlCode(unsigned long code) {
//         SERVICE_STATUS status{};
//         return m_service.Call(&ControlService, code, &status);
//     }

// private:
//     // SC_HANDLE CreateLeagueService() {
//     //     char target[MAX_PATH] = {};
//     //     GetFullPathNameA("lppservice.exe", MAX_PATH, target, nullptr);
//     //     return CreateServiceA(m_manager,
//     //         SERVICE_NAME,
//     //         SERVICE_NAME_DETAIL,
//     //         SC_MANAGER_ALL_ACCESS,
//     //         SERVICE_WIN32_OWN_PROCESS,
//     //         SERVICE_DEMAND_START,
//     //         SERVICE_ERROR_NORMAL,
//     //         target,
//     //         nullptr, nullptr, nullptr, nullptr, nullptr);
//     // }

// private:
//     win32::sc_handle m_manager {};
//     win32::sc_handle m_service {};
// };

class ServiceIOClient : public IServiceIO {
public:
    ServiceIOClient(win32::handle handle) : IServiceIO(std::move(handle)) {}

    static ServiceIOClient Create(void) {
        return ServiceIOClient(win32::handle(ConnectServicePipe()));
    }
};

int main(int argc, char** argv) {
    for (int i = 0; i < argc; i++)
        std::cout << "arg: " << argv[i] << "\n";

    win32::sc_handle svcManager = OpenSCManagerA(nullptr, SERVICES_ACTIVE_DATABASEA, SC_MANAGER_CONNECT);
    win32::sc_handle svc = svcManager.Call(&OpenServiceA, SERVICE_NAME, SERVICE_START | SERVICE_STOP | SERVICE_QUERY_STATUS);

    // auto serviceManager = ServiceManager();
    // if (!serviceManager.Init()) {
    //     std::cout << "failed to init sm\n" << GetLastError() << "\n";
    //     return false;
    // }

    // while (serviceManager.QueryState() != SERVICE_RUNNING)
    //     Sleep(100);

    // auto client = ServiceIOClient::Create();

    // std::cout << client.SendPacket(1234, 10) << "\n";

    // // Sleep(5000);

    // std::cout << serviceManager.SendControlCode(SERVICE_CONTROL_STOP) << "\n";

    return 0;

    // WINDOW_CONFIG config{};
    // config.flags = { WINDOW_FLAG_SHOW, WINDOW_FLAG_HIDE_TITLE_BAR };
    // config.size = { 700, 600 };
    // config.name = "testwidow";
    // config.iconId = 1;
    // config.iconName = "disc_icon_2.png";

    // BROWSER_CONFIG bwConfig{};
    // bwConfig.flags = { BROWSER_FLAG_DEV_TOOLS };

    // return 0;
}