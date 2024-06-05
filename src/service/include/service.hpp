#pragma once

#ifndef __SERVICE_HPP__
#define __SERVICE_HPP__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <sstream>
#include <format>
#include <shared/win32.hpp>

// ~~ shared service header
//    global consts & methods

// ~~ global constants
#define SERVICE_NAME                "lppservice"
#define SERVICE_NAME_DETAIL         "league++ service handler"
#define SERVICE_PIPE_NAME           "\\\\.\\pipe\\lppservice"
#define SERVICE_PACKET_DATA_SIZE    512ull

// debug stream with prefix for this service
inline win32::DebugStream service_debug { std::format("[{}]: ", SERVICE_NAME_DETAIL) };

// this is the "packet" that transfers betweens the read/write file
struct service_packet {
    // command number from 0 to max u32
    uint32_t m_cmd;
    // the size of the data from 0 to SERVICE_PACKET_DATA_SIZE
    uint32_t m_size;
    // data buffer of size SERVICE_PACKET_DATA_SIZE
    char m_data[SERVICE_PACKET_DATA_SIZE];
};

// a wrapper class for handling the ipc
// [ non copyable ]
class IServiceIO {
public:
    IServiceIO(win32::handle handle) : m_handle(std::move(handle)) {}
    ~IServiceIO(void) = default;

    IServiceIO(const IServiceIO&) = delete;
    IServiceIO& operator=(const IServiceIO& rhs) = delete;

    template <typename Ty>
    bool SendPacket(uint32_t cmd, const Ty* data, uint32_t size = sizeof(Ty)) {
        // init packet
        service_packet packet{};

        // set command
        packet.m_cmd = cmd;
        
        // set data
        if (!SetPacketData(packet, data, size))
            return false;
    
        // write to remote
        return WriteFile(m_handle.GetHandleRef(), &packet, sizeof(packet), nullptr, nullptr);
    }

    template <typename Ty>
    bool SendPacket(uint32_t cmd, const Ty& data, uint32_t size = sizeof(Ty)) {
        // stupid wrapper
        return SendPacket(cmd, &data, size);
    }

    bool RecvPacket(service_packet& packet) {
        // clear data field
        memset(packet.m_data, 0, SERVICE_PACKET_DATA_SIZE);

        // load the object
        return ReadFile(m_handle.GetHandleRef(), &packet, sizeof(packet), nullptr, nullptr);
    }

protected:
    win32::handle m_handle;
};

template <typename Ty>
bool SetPacketData(service_packet& packet, const Ty* data, uint32_t size = sizeof(Ty)) {
    // double check the size 
    if (size > SERVICE_PACKET_DATA_SIZE)
        return false;

    // set size
    packet.m_size = size;

    // set data
    memset(packet.m_data, 0, SERVICE_PACKET_DATA_SIZE);
    memcpy(packet.m_data, data, size);
    return true;
}

// SECTION: yikes winapi

inline HANDLE CreateServicePipe() {
    return CreateNamedPipeA(SERVICE_PIPE_NAME,
            PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED | FILE_FLAG_FIRST_PIPE_INSTANCE,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            1,
            sizeof(service_packet),
            sizeof(service_packet),
            NMPWAIT_USE_DEFAULT_WAIT,
            nullptr);
}

inline HANDLE ConnectServicePipe() {
    return CreateFileA(SERVICE_PIPE_NAME,
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr);
}

#endif // __SERVICE_HPP__