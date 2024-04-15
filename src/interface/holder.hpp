#ifndef __HOLDER_HPP__
#define __HOLDER_HPP__

#include <memory>
#undef interface

// #include "../hash.hpp"
// #define INTERFACE_HOLDER(name) \
//     struct name##_t {}; \
//     inline constexpr name##_t name##{}
// INTERFACE_HOLDER(as_unique);
// INTERFACE_HOLDER(from_singleton);

template <typename Ty>
class interface {
public:
    static constexpr std::shared_ptr<Ty> Get() {
        const static std::shared_ptr<Ty> ptr(new Ty());
        return ptr;
    }

    template <typename... Args>
    static constexpr std::shared_ptr<Ty> Create(Args&&... args) {
        return std::shared_ptr<Ty>(new Ty(std::forward<Args>(args)...));
    }
};

#endif // __HOLDER_HPP__