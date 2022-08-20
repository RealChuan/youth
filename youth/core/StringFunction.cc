#include "StringFunction.hpp"
#include <string>
#include <string_view>

namespace youth {

namespace core {

namespace string {

std::vector<std::string> split(const std::string &text, const std::string &s)
{
    std::vector<std::string> result;
    std::string::size_type pos1 = 0;
    std::string::size_type pos2 = text.find(s);
    while (pos2 != std::string::npos) {
        result.push_back(text.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = text.find(s, pos1);
    }
    result.push_back(text.substr(pos1));
    return result;
}

bool remove(std::string &text, const std::string &s)
{
    std::string::size_type pos = text.find(s);
    if (pos != std::string::npos) {
        text.erase(pos, s.length());
        return true;
    }
    return false;
}

bool replace(std::string &text, const std::string &s, const std::string &r)
{
    std::string::size_type pos = text.find(s);
    if (pos != std::string::npos) {
        text.replace(pos, s.length(), r);
        return true;
    }
    return false;
}

bool trimmed(const std::string &text)
{
    if (text.empty()) {
        return false;
    }
    std::string::size_type pos = text.find_first_not_of(" \t\n\r");
    if (pos == std::string::npos) {
        return false;
    }
    std::string::size_type pos2 = text.find_last_not_of(" \t\n\r");
    if (pos2 == std::string::npos) {
        return false;
    }
    return true;
}

bool toLower(std::string &text)
{
    if (text.empty()) {
        return false;
    }
    for (std::string::size_type i = 0; i < text.length(); i++) {
        text[i] = tolower(text[i]);
    }
    return true;
}

bool toUpper(std::string &text)
{
    if (text.empty()) {
        return false;
    }
    for (std::string::size_type i = 0; i < text.length(); i++) {
        text[i] = toupper(text[i]);
    }
    return true;
}

bool startsWith(const std::string &text, const std::string &str)
{
    return text.find(str) == 0;
}

bool endsWith(const std::string &text, const std::string &str)
{
    return text.rfind(str) == text.length() - str.length();
}

bool contains(const std::string &text, const std::string &str)
{
    return text.find(str) != std::string::npos;
}

bool isLower(const std::string &text)
{
    for (auto c : text) {
        if (c < 'a' || c > 'z') {
            return false;
        }
    }
    return true;
}

bool isUpper(const std::string &text)
{
    for (auto c : text) {
        if (c < 'A' || c > 'Z') {
            return false;
        }
    }
    return true;
}

} // namespace string

} // namespace core

} // namespace youth
