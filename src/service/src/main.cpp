#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "service.hpp"
#include "server.hpp"

class LeagueServiceHandler {
public:
    static void Main(unsigned long, char**) {
        // setup io server
        m_server = ServiceIOServer::Create();

        // register winapi stuff
        m_handle = RegisterServiceCtrlHandlerA(SERVICE_NAME, LeagueServiceHandler::CtrlHandler);
        m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
        m_status.dwServiceSpecificExitCode = 0;
        SetStatus(1, SERVICE_RUNNING, SERVICE_ACCEPT_STOP);

        // main loop
        m_server->Start([&](const service_packet* packet) {
            service_debug << "recieved packet (cmd: " << packet->m_cmd << ")" << std::endl;
        });

        // exit
        SetStatus(2, SERVICE_STOPPED);
        m_server = nullptr;
    }

private:
    static void CtrlHandler(DWORD code) {
        switch (code) {
            case SERVICE_CONTROL_STOP:
                if (m_status.dwCurrentState != SERVICE_RUNNING)
                    break;

                SetStatus(3, SERVICE_STOP_PENDING);
                m_server->CallStopEvent();
                break;
            default:
                break;
        }
    }

    static void SetStatus(uint32_t point, uint32_t state, uint32_t controls = 0, uint32_t exit = 0) {
        m_status.dwCheckPoint = point;
        m_status.dwCurrentState = state;
        m_status.dwControlsAccepted = controls;
        m_status.dwWin32ExitCode = exit;
        SetServiceStatus(m_handle, &m_status);
    }

private:
    inline static SERVICE_STATUS m_status = { 0 };
    inline static SERVICE_STATUS_HANDLE m_handle = nullptr;
    inline static std::unique_ptr<ServiceIOServer> m_server;
};

int main() {
    SERVICE_TABLE_ENTRYA ServiceTable[] = {
        { (char*)SERVICE_NAME, LeagueServiceHandler::Main },
        { nullptr, nullptr }
    };

    if (StartServiceCtrlDispatcherA(ServiceTable) == FALSE) {
        std::string message;

        switch (auto code = GetLastError()) {
            case ERROR_FAILED_SERVICE_CONTROLLER_CONNECT:
                service_debug << "error: the service process could not connect to the service controller" << std::endl;
                return code;
            case ERROR_SERVICE_ALREADY_RUNNING:
                service_debug << "error: an instance of the service is already running" << std::endl;
                return code;
            default:
                service_debug << "error: failed to create service instance" << std::endl;
                return code;
        }
    }

    return 0;
}