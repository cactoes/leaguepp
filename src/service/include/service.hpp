#pragma once

#ifndef __SERVICE_HPP__
#define __SERVICE_HPP__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// ~~ shared service header
//    global consts & methods

#define SERVICE_NAME (char*)"lppservice"
#define SERVICE_NAME_DETAIL (char*)"league++ service handler"
#define SERVICE_PIPE_NAME (char*)"\\\\.\\pipe\\lppservice"
#define SERVICE_PACKET_DATA_SIZE 512

struct SERVICE_PACKET {
    unsigned long m_cmd;
    unsigned long m_size;
    char m_data[SERVICE_PACKET_DATA_SIZE];
};

template <typename Ty>
bool SetPacketData(SERVICE_PACKET& packet, const Ty* data, unsigned long size = sizeof(Ty)) {
    if (size > SERVICE_PACKET_DATA_SIZE)
        return false;

    packet.m_size = size;
    memset(packet.m_data, 0, SERVICE_PACKET_DATA_SIZE);
    memcpy(packet.m_data, data, size);
    return true;
}

template <typename Ty>
bool SetPacketData(SERVICE_PACKET& packet, const Ty& data, unsigned long size = sizeof(Ty)) {
    return SetPacketData(packet, &data, size);
}

class IServiceIO {
public:
    IServiceIO(HANDLE handle) : m_handle(handle) {}
    ~IServiceIO(void) { CloseHandle(m_handle); }

    template <typename Ty>
    bool SendPacket(unsigned long cmd, const Ty* data, unsigned long size = sizeof(Ty)) {
        SERVICE_PACKET packet{};
        packet.m_cmd = cmd;
        
        if (!SetPacketData(packet, data, size))
            return false;
    
        unsigned long _nobw;
        return WriteFile(m_handle, &packet, sizeof(packet), &_nobw, nullptr);
    }

    template <typename Ty>
    bool SendPacket(unsigned long cmd, const Ty& data, unsigned long size = sizeof(Ty)) {
        return SendPacket(cmd, &data, size);
    }

    bool RecvPacket(SERVICE_PACKET& packet) {
        memset(packet.m_data, 0, SERVICE_PACKET_DATA_SIZE);
        unsigned long _nobr;
        return ReadFile(m_handle, &packet, sizeof(packet), &_nobr, nullptr);
    }

protected:
    HANDLE m_handle;
};

#endif // __SERVICE_HPP__