#ifndef __TYPEHELPERS_HPP__
#define __TYPEHELPERS_HPP__

#include <Windows.h>
#include <string>
#include <any>

static std::wstring StringToWideString(const std::string_view string) {
    if (string.empty()) {
        return {};
    }

    const int stringLength = MultiByteToWideChar(CP_UTF8, 0, string.data(), static_cast<int>(string.size()), nullptr, 0);
    std::wstring wstring(stringLength, 0);
    MultiByteToWideChar(CP_UTF8, 0, string.data(), static_cast<int>(string.size()), wstring.data(), stringLength);
    return wstring;
}
static std::string WideStringToString(const std::wstring_view wstring) {
    if (wstring.empty()) {
        return {};
    }

    const int stringLength = WideCharToMultiByte(CP_UTF8, 0, wstring.data(), static_cast<int>(wstring.size()), nullptr, 0, nullptr, nullptr);
    std::string string(stringLength, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstring.data(), static_cast<int>(wstring.size()), string.data(), stringLength, nullptr, nullptr);
    return string;
}
static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length();
    }
    return str;
}

static std::string AnyToMappedString(std::any value) {
    if (value.type() == typeid(int))
        return std::to_string(std::any_cast<int>(value));

    if (value.type() == typeid(float))
        return std::to_string(std::any_cast<float>(value));

    if (value.type() == typeid(bool))
        return std::to_string(std::any_cast<bool>(value));

    if (value.type() == typeid(std::string)) {
        auto str = std::any_cast<std::string>(value);
        str = ReplaceAll(str, "\"", "\\\"");
        return "\"" + str + "\"";
    }

    if (value.type() == typeid(const char*)) {
        std::string str = std::any_cast<const char*>(value);
        str = ReplaceAll(str, "\"", "\\\"");
        return "\"" + str + "\"";
    }

    if (value.type() == typeid(std::vector<std::any>)) {
        std::string str = {};

        for (const auto& v : std::any_cast<std::vector<std::any>>(value))
            str += AnyToMappedString(v) + ",";

        return "[" + str + "]";
    }

    if (value.type() == typeid(std::vector<std::string>)) {
        std::string str = {};

        for (const auto& v : std::any_cast<std::vector<std::string>>(value))
            str += "\"" + v + "\",";

        return "[" + str + "]";
    }

    if (value.type() == typeid(std::vector<bool>)) {
        std::string str = {};

        for (const auto& v : std::any_cast<std::vector<bool>>(value))
            str += std::to_string(v) + ",";

        return "[" + str + "]";
    }

    if (value.type() == typeid(std::vector<int>)) {
        std::string str = {};

        for (const auto& v : std::any_cast<std::vector<int>>(value))
            str += std::to_string(v) + ",";

        return "[" + str + "]";
    }

    if (value.type() == typeid(std::vector<float>)) {
        std::string str = {};

        for (const auto& v : std::any_cast<std::vector<float>>(value))
            str += std::to_string(v) + ",";

        return "[" + str + "]";
    }

    return "undefined";
}

#endif // __TYPEHELPERS_HPP__