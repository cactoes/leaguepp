#pragma once

#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include <string>
#include <memory>
#include <shared/random.hpp>
#include <shared/callback.hpp>

// ~~ the component base

// (internal) shorthand for creating unique ids
#define COMPONENT_CALLER_ID(callerType) GetId() + #callerType

// struct to contain dimensional data
struct VECTOR2I {
    int x, y;
};

namespace component {
    class IComponent {
    public:
        virtual ~IComponent(void) = default;

        virtual void Update(void) = 0;
        virtual void Render(void) = 0;

        void SetId(const std::string& id) { m_id = id; }
        const std::string& GetId(void) const { return m_id; }

        void SetTarget(const std::string& target) { m_target = target; }

    protected:
        std::string m_name;
        std::string m_target;

    private:
        std::string m_id = random::UUID();
    };
}; // namespace component

#endif // __COMPONENT_HPP__