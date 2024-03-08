#ifndef __HOLDER_HPP__
#define __HOLDER_HPP__

#include <memory>
#undef interface

#include "../hash.hpp"

#define INTERFACE_HOLDER(name) \
    struct name##_t {}; \
    inline constexpr name##_t name##{}

INTERFACE_HOLDER(as_unique);
INTERFACE_HOLDER(from_singleton);

namespace interface {
    template <typename Ty, typename... Args>
    constexpr std::shared_ptr<Ty> GetHolder(as_unique_t, Args&&... args) {
        return std::shared_ptr<Ty>(new Ty(std::forward<Args>(args)...));
    }

    template <typename Ty>
    constexpr std::shared_ptr<Ty> GetHolder(from_singleton_t) {
        const static std::shared_ptr<Ty> ptr(new Ty());
        return ptr;
    }
}; // namespace interface

#endif // __HOLDER_HPP__