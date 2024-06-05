#pragma once

#ifndef __BITFIELD_HPP__
#define __BITFIELD_HPP__

#include <string>
#include <array>

// ~~ bitfield helper class

class BitField {
public:
    typedef uint64_t field_type;
    static constexpr int nmax = sizeof(field_type) * 8;

    BitField(void) = default;

    BitField(const std::initializer_list<field_type>& fields) {
        for (field_type v : fields)
            Set(v);
    }

    template <uint64_t N>
    bool Set(void) { return Set(N); }

    template <uint64_t N>
    bool Clear(void) { return Clear(N); }

    template <uint64_t N>
    bool Has(void) const { return Has(N); }

    auto Get(void) { return m_field; }

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
    bool is_valid_index(uint64_t n) const { return n < 0 || n > (nmax - 1); }

    bool Set(uint64_t n) {
        if (is_valid_index(n))
            return false;

        m_field |= ((field_type)1 << n);
        return true;
    }
    
    bool Clear(uint64_t n) {
        if (is_valid_index(n))
            return false;

        m_field &= ~((field_type)1 << n);
        return true;
    }

    bool Has(uint64_t n) const {
        if (is_valid_index(n))
            return false;

        return (m_field >> n) & (field_type)1;
    }

private:
    field_type m_field = (field_type)0;
};

#endif // __BITFIELD_HPP__