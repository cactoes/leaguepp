#ifndef __EVENTBUS_HPP__
#define __EVENTBUS_HPP__

#include <mutex>
#include <vector>
#include <queue>
#include <string>
#include <any>

class EventBus {
public:
    struct message_t {
        std::string name;
        std::vector<std::any> args;
    };

public:
    void AddMessage(const message_t& message) {
        std::lock_guard<std::mutex> guard(m_mutex);
        m_bus.push(message);
    }

    bool Get(message_t* pMessage) {
        std::lock_guard<std::mutex> guard(m_mutex);

        if (m_bus.empty())
            return false;

        *pMessage = m_bus.front();
        m_bus.pop();

        return true;
    }

private:
    std::queue<message_t> m_bus = {};
    std::mutex m_mutex;
};

#endif // __EVENTBUS_HPP__