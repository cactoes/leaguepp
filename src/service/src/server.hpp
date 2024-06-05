#pragma once

#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include "service.hpp"

#define STATUS_CODE(n, v) constexpr static auto (n) = (uint32_t)(v)

struct SIOSST {
    STATUS_CODE(INVALID, 0);
    STATUS_CODE(SHOULD_QUIT, 1);
    STATUS_CODE(HAS_PACKET, 2);
    STATUS_CODE(IDLE, 3);
};

class ServiceIOServer : public IServiceIO {
public:
    ServiceIOServer(win32::handle handle) : IServiceIO(std::move(handle)),
        m_overlappedEvent(CreateEventA(nullptr, true, false, nullptr)),
        m_stopEvent(CreateEventA(nullptr, true, false, nullptr)) {
        m_overlapped.hEvent = m_overlappedEvent.GetHandleRef();
    }

    static std::unique_ptr<ServiceIOServer> Create() {
        return std::make_unique<ServiceIOServer>(win32::handle(CreateServicePipe()));
    }

    void Start(auto&& PacketHandler)
        requires std::invocable<decltype(PacketHandler), const service_packet*>
    {
        for (auto status = PollEvent(); status != SIOSST::INVALID && status != SIOSST::SHOULD_QUIT; status = PollEvent()) {
            switch (status) {
                case SIOSST::HAS_PACKET:
                    PacketHandler(&m_currentPacket);
                    break;
                case SIOSST::IDLE:
                default:
                    break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    void CallStopEvent() {
        m_stopEvent.Call(&SetEvent);
    }

private:
    uint32_t PollEvent(void) {
        // validate handles
        if (!m_handle.IsValid() || !m_stopEvent.IsValid() || !m_overlappedEvent.IsValid())
            return SIOSST::INVALID;

        // reset overlap event
        m_overlappedEvent.Call(&ResetEvent);

        // check incoming connection
        if (m_handle.Call(&ConnectNamedPipe, &m_overlapped) == FALSE) {
            switch (GetLastError()) {
                case ERROR_PIPE_CONNECTED:
                    m_overlappedEvent.Call(&ResetEvent);
                    break;
                case ERROR_NO_DATA:
                    m_handle.Call(&DisconnectNamedPipe);
                    return SIOSST::IDLE;
                case ERROR_IO_PENDING:
                    break;
                default:
                    return SIOSST::IDLE;
            }
        }

        // wait for either a stop event or incoming connection
        HANDLE m_events[] = { m_stopEvent.GetHandleRef(), m_overlappedEvent.GetHandleRef() };
        unsigned long waitObj = WaitForMultipleObjects(2, m_events, false, INFINITE);

        // it was stop
        if (waitObj == WAIT_OBJECT_0)
            return SIOSST::SHOULD_QUIT;

        // it was data yy
        if (waitObj == WAIT_OBJECT_0 + 1) {
            if (RecvPacket(m_currentPacket))
                return SIOSST::HAS_PACKET;

            m_handle.Call(&DisconnectNamedPipe);
        }

        // it was neither
        return SIOSST::IDLE;
    }
    
private:
    win32::handle m_stopEvent;
    win32::handle m_overlappedEvent;
    service_packet m_currentPacket = {};
    OVERLAPPED m_overlapped = { 0 };
};

#endif // __SERVER_HPP__