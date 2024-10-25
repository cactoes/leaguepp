#pragma once

#ifndef __LPP_CORE_UTILS_HPP__
#define __LPP_CORE_UTILS_HPP__

#include <optional>
#include <regex>
#include <string>

#define lpp_c_u_no_discard [[nodiscard]]

namespace lpp_core_utils {
    template <typename _number_type>
        requires std::is_arithmetic_v<_number_type>
    inline std::optional<_number_type> parse_number(const std::string& str) {
        _number_type num {};
        const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), num);
        return ec == std::errc() ? std::optional(num) : std::nullopt;
    }

    lpp_c_u_no_discard inline bool test(const std::string& text, const std::string& pattern) {
        return std::regex_match(text, std::regex(pattern));
    }

    lpp_c_u_no_discard inline bool strcmpi(const std::string_view& a, const std::string_view& b) {
        return std::ranges::equal(a, b, [](char a, char b) {
            return std::tolower(static_cast<int>(a)) == std::tolower(static_cast<int>(b));
        });
    }
} // namespace lpp_core_utils

#endif // __LPP_CORE_UTILS_HPP__