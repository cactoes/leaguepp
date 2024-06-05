#pragma once

#ifndef __WIN32_HPP__
#define __WIN32_HPP__

#include <Windows.h>

// ~~ win32 wrapper functions

namespace win32 {
    namespace detail {
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
    }; // namespace detail

    using namespace detail;

    typedef AutoHandle<HANDLE, &CloseHandle> handle;
    typedef AutoHandle<SC_HANDLE, &CloseServiceHandle> sc_handle;

    DebugStream debug {};
}; // namespace win32

#endif // __WIN32_HPP__