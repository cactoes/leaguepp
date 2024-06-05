#pragma once

#ifndef __WIN32_HPP__
#define __WIN32_HPP__

#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <ShlObj.h>

// ~~ win32 wrapper functions

namespace win32 {
    // self closing handle wrapper for winapi handle types
    // [ non copyable ]
    template <typename handle_type, typename BOOL(WINAPI* closer)(handle_type)>
    class AutoHandle {
    public:
        using ht = handle_type;

        explicit AutoHandle(void) : m_handle(nullptr) {}
        AutoHandle(handle_type handle) : m_handle(handle) {}
        ~AutoHandle(void) { (*closer)(m_handle); }

        AutoHandle(const AutoHandle&) = delete;
        AutoHandle& operator=(const AutoHandle& rhs) = delete;

        AutoHandle(AutoHandle&& r) {
            m_handle = r.m_handle;
            r.m_handle = nullptr;
        };

        AutoHandle& operator=(AutoHandle&& r) {
            if (this != &r) {
                this->~AutoHandle();
                m_handle = r.m_handle;
                r.m_handle = nullptr;
            }
            return *this;
        };

        template <typename... Args>
        auto Call(auto&& Function, Args&&... args)
            requires std::invocable<decltype(Function), handle_type, Args...> {
            return Function(m_handle, std::forward<Args>(args)...);
        }

        bool IsValid() {
            return m_handle != nullptr && m_handle != INVALID_HANDLE_VALUE;
        }

        handle_type GetHandleRef() {
            return m_handle;
        }

        handle_type* GetHandlePtr() {
            return &m_handle;
        }

    private:
        handle_type m_handle;
    };

    // stream wrapper for OutputDebugStringA
    class DebugStream {
    public:
        DebugStream(void) = default;
        DebugStream(const std::string& prefix) : m_prefix(prefix) {};

        template<typename T>
        DebugStream& operator<<(const T& data) {
            m_buffer << data;
            return *this;
        }

        DebugStream& operator<<(std::ostream& (*func)(std::ostream&)) {
            if (func == static_cast<std::ostream& (*)(std::ostream&)>(std::endl))
                flush();
            
            return *this;
        }

        void flush() {
            if (std::string message = m_buffer.str(); !message.empty()) {
                if (!m_prefix.empty())
                    message = m_prefix + message;

                OutputDebugStringA((message + "\n").c_str());
                std::cout << message << std::endl;
                m_buffer.str("");
                m_buffer.clear();
            }
        }

    private:
        std::ostringstream m_buffer;
        std::string m_prefix;
    };

    typedef AutoHandle<HANDLE, &CloseHandle> handle;
    typedef AutoHandle<SC_HANDLE, &CloseServiceHandle> sc_handle;
    typedef AutoHandle<HKEY, (BOOL (__stdcall*)(HKEY))&RegCloseKey> hkey;

    DebugStream debug {};

    // registry key wrapper class
    class RegistryEntry {
    public:
        RegistryEntry(const std::string& path) : m_path(path) {}

        bool Init(bool createIfMissing = false) {
            auto openKeyResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, m_path.c_str(), 0, KEY_ALL_ACCESS, m_key.GetHandlePtr());

            if (openKeyResult == ERROR_FILE_NOT_FOUND) {
                if (!createIfMissing)
                    return false;

                if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, m_path.c_str(), 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, m_key.GetHandlePtr(), nullptr) != ERROR_SUCCESS)
                    return false;
            }

            return openKeyResult == ERROR_SUCCESS;
        }

        template <DWORD type, typename value_type>
        bool SetRegValue(const char* name, const value_type* data, DWORD size) {
            return m_key.Call(&RegSetValueExA, name, 0, type, (BYTE*)data, size) == ERROR_SUCCESS;
        }

        template <DWORD type>
        bool SetRegValue(const char* name, const std::string& data) {
            return SetRegValue<type>(name, data.data(), (DWORD)(data.size() + 1));
        }

        template <DWORD type>
        bool SetRegValue(const char* name, const DWORD& data) {
            return SetRegValue<type>(name, &data, sizeof(DWORD));
        }

        template <typename type>
        bool GetRegValue(const char* name, type* out, DWORD& size = sizeof(type)) {
            return m_key.Call(&RegQueryValueExA, name, nullptr, nullptr, (BYTE*)out, &size) == ERROR_SUCCESS;
        }

        bool GetRegValue(const char* name, std::string& out) {
            DWORD size = MAX_PATH;
            char buff[MAX_PATH];
            if (!GetRegValue(name, buff, size))
                return false;

            out.assign(buff, buff + size - 1);
            return true;
        }

    private:
        std::string m_path = "";
        win32::hkey m_key = nullptr;
    };

    
    // http://alter.org.ua/en/docs/win/args/
    std::vector<std::string> CommandLineToArgvExA(const std::string& cmdLine) {
        std::vector<std::string> argv;
        std::string currentArg;
        bool inQuote = false;
        bool inText = false;
        bool inSpace = true;

        for (char a : cmdLine) {
            if (inQuote) {
                if (a == '\"')
                    inQuote = false;
                else
                    currentArg += a;
                continue;
            }

            switch (a) {
                case '\"':
                    inQuote = true;
                    inText = true;
                    if (inSpace) {
                        if (!currentArg.empty()) {
                            argv.push_back(currentArg);
                            currentArg.clear();
                        }
                    }
                    inSpace = false;
                    break;
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    if (inText && !currentArg.empty()) {
                        argv.push_back(currentArg);
                        currentArg.clear();
                    }
                    inText = false;
                    inSpace = true;
                    break;
                default:
                    inText = true;
                    if (inSpace && !currentArg.empty()) {
                        argv.push_back(currentArg);
                        currentArg.clear();
                    }
                    currentArg += a;
                    inSpace = false;
                    break;
            }
        }

        if (!currentArg.empty()) {
            argv.push_back(currentArg);
        }

        return argv;
    }

    std::filesystem::path GetFolderPath(int csidl) {
        char path[MAX_PATH];
        SHGetFolderPathA(nullptr, csidl, nullptr, 0, path);
        return std::filesystem::path(path);
    }
}; // namespace win32

#endif // __WIN32_HPP__