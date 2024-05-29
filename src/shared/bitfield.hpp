#pragma once

#ifndef __BITFIELD_HPP__
#define __BITFIELD_HPP__

#include <string>
#include <array>

// ~~ bitfield helper class

template <typename Ty = unsigned long, int nmax = sizeof(Ty) * 8>
class BitField {
public:
    BitField(void) = default;

    template <int N>
    BitField(std::array<Ty, N> fields) {
        for (Ty v : fields)
            Set(v);
    }

    template <typename... Args, typename = std::enable_if_t<std::conjunction_v<std::is_same<Args, Ty>...>>>
    BitField(Args... args) {
        (Set(args), ...);
    }

    template <Ty N>
    bool Set(void) { return Set(N); }

    template <Ty N>
    bool Clear(void) { return Clear(N); }

    template <Ty N>
    bool Has(void) const { return Has(N); }

    Ty Get(void) {
        return m_field;
    }

    std::string ToString(void) const {
        std::string out = "[ ";
        for (int i = 0; i < nmax; i++) {
            out += std::to_string(Has(i));
            out += ", ";
        }
        out += "]";

        return out;
    }

private:
    bool Set(Ty n) {
        if (n < 0 || n > (nmax - 1))
            return false;

        m_field |= ((Ty)1 << n);
        return true;
    }
    
    bool Clear(Ty n) {
        if (n < 0 || n > (nmax - 1))
            return false;

        m_field &= ~((Ty)1 << n);
        return true;
    }

    bool Has(Ty n) const {
        if (n < 0 || n > (nmax - 1))
            return false;

        return (m_field >> n) & (Ty)1;
    }

private:
    Ty m_field = (Ty)0;
};

#endif // __BITFIELD_HPP__