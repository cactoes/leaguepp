#include <iostream>

#include "uiframework.hpp"

#include <Windows.h>

int main(int, char**) {
    auto message_box = CreateMessageBox("error", "error message", message_box_type::MB_ERROR);
    auto message_box1 = CreateMessageBox("warn", "warn message", message_box_type::MB_WARN);
    auto message_box2 = CreateMessageBox("info", "info message", message_box_type::MB_INFO);

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
