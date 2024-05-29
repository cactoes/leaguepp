#include <iostream>
#include <memory>
#include <array>
#include <vector>
#include <string>

#include "service.hpp"
#include "bitfield.hpp"

#define SIOSST_ERR_BIT 31ul
#define SIOSST_INVALID_BIT 0ul
#define SIOSST_SHOULD_QUIT_BIT 1ul
#define SIOSST_HAS_PACKET_BIT 2ul
#define SIOSST_IDLE_BIT 3ul

inline void SystemLogMessage(const std::string type, const std::string& message) {
    std::string msg = "[league++ service (" + type + ")] " + message;
    std::cout << msg << "\n";
    OutputDebugStringA(msg.c_str());
}

struct SERVICE_CONTAINER {
    SERVICE_STATUS m_status = { 0 };
    SERVICE_STATUS_HANDLE m_handle = nullptr;
    HANDLE m_stopEvent = INVALID_HANDLE_VALUE;
} g_serviceContainer;

class ServiceIOServer : public IServiceIO {
public:
    ServiceIOServer(HANDLE handle, HANDLE stopEvent) : IServiceIO(handle) {
        m_overlapped.hEvent = CreateEventA(nullptr, true, false, nullptr);
        m_events = { stopEvent,  m_overlapped.hEvent };
    }
    
    ~ServiceIOServer(void) {
        CloseHandle(m_overlapped.hEvent);
    }

    static ServiceIOServer Create(HANDLE stopEvent) {
        HANDLE handle = CreateNamedPipeA(SERVICE_PIPE_NAME,
            PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED | FILE_FLAG_FIRST_PIPE_INSTANCE,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            1,
            sizeof(SERVICE_PACKET),
            sizeof(SERVICE_PACKET),
            NMPWAIT_USE_DEFAULT_WAIT,
            nullptr);

        return ServiceIOServer(handle, stopEvent);
    }

    BitField<unsigned long> PollEvent(void) {
        if (m_handle == INVALID_HANDLE_VALUE || m_events.at(0) == INVALID_HANDLE_VALUE || m_overlapped.hEvent == INVALID_HANDLE_VALUE)
            return { SIOSST_ERR_BIT, SIOSST_INVALID_BIT };

        ResetEvent(m_overlapped.hEvent);

        if (ConnectNamedPipe(m_handle, &m_overlapped) == FALSE) {
            unsigned long errc = GetLastError();
            if (errc == ERROR_PIPE_CONNECTED)
                ResetEvent(m_overlapped.hEvent);
            else if (errc != ERROR_IO_PENDING)
                return { SIOSST_IDLE_BIT };
        }

        unsigned long waitObj = WaitForMultipleObjects((unsigned long)m_events.size(), m_events.data(), false, INFINITE);

        if (waitObj == WAIT_OBJECT_0)
            return { SIOSST_ERR_BIT, SIOSST_SHOULD_QUIT_BIT };

        if (waitObj == WAIT_OBJECT_0 + 1) {
            if (SERVICE_PACKET packet{}; RecvPacket(packet)) {
                m_currentPacket = packet;
                return { SIOSST_HAS_PACKET_BIT };
            }

            DisconnectNamedPipe(m_handle);
        }

        return { SIOSST_IDLE_BIT };
    }

    const SERVICE_PACKET& GetPacket(void) {
        return m_currentPacket;
    }

private:
    SERVICE_PACKET m_currentPacket = {};
    // HANDLE m_stopEvent;
    std::vector<HANDLE> m_events = {};
    OVERLAPPED m_overlapped = { 0 };
};

inline void SetServiceStatus(SERVICE_CONTAINER& container, unsigned long point, unsigned long state, unsigned long controls = 0, unsigned long exit = 0) {
    container.m_status.dwCheckPoint = point;
    container.m_status.dwCurrentState = state;
    container.m_status.dwControlsAccepted = controls;
    container.m_status.dwWin32ExitCode = exit;
    SetServiceStatus(container.m_handle, &container.m_status);
}

void ServiceCtrlHandler(unsigned long code) {
    switch (code) {
        case SERVICE_CONTROL_STOP:
            if (g_serviceContainer.m_status.dwCurrentState != SERVICE_RUNNING)
                break;

            SetServiceStatus(g_serviceContainer, 3, SERVICE_STOP_PENDING);
            SetEvent(g_serviceContainer.m_stopEvent);
            break;
        default:
            break;
    }
}

void ServiceMainA(unsigned long, char**) {
    g_serviceContainer.m_handle = RegisterServiceCtrlHandlerA(SERVICE_NAME, ServiceCtrlHandler);
    g_serviceContainer.m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_serviceContainer.m_status.dwServiceSpecificExitCode = 0;
    g_serviceContainer.m_stopEvent = CreateEventA(nullptr, true, false, nullptr);
    SetServiceStatus(g_serviceContainer, 0, SERVICE_START_PENDING);

    auto server = ServiceIOServer::Create(g_serviceContainer.m_stopEvent);

    SetServiceStatus(g_serviceContainer, 1, SERVICE_RUNNING, SERVICE_ACCEPT_STOP);

    BitField status = server.PollEvent();
    while (!status.Has<SIOSST_ERR_BIT>()) {

        if (status.Has<SIOSST_HAS_PACKET_BIT>()) {
            auto packet = server.GetPacket();
            SystemLogMessage("core", "recieved packet (cmd: " + std::to_string(packet.m_cmd) + ")");
        }

        status = server.PollEvent();
    }

    // should server take control over ts?
    CloseHandle(g_serviceContainer.m_stopEvent);
    SetServiceStatus(g_serviceContainer, 2, SERVICE_STOPPED);
}

int main() {
    SERVICE_TABLE_ENTRYA ServiceTable[] = {
        { SERVICE_NAME, ServiceMainA },
        { nullptr, nullptr }
    };

    if (StartServiceCtrlDispatcherA(ServiceTable) == FALSE) {
        std::string message;

        switch (GetLastError()) {
            case ERROR_FAILED_SERVICE_CONTROLLER_CONNECT:
                message = "error: this program cannot be run in console mode";
                break;
            case ERROR_SERVICE_ALREADY_RUNNING:
                message = "error: service already running";
                break;
            default:
                message = "error: failed to start service dispatcher";
                break;
        }

        SystemLogMessage("startup", message);
    }

    return 0;
}