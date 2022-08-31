#include "StringFunction.hpp"

#include <algorithm>
#include <string>
#include <string_view>

namespace youth {

namespace core {

namespace string {

std::vector<std::string> split(std::string_view text, std::string_view s)
{
    std::vector<std::string> result;
    std::string::size_type pos1 = 0;
    std::string::size_type pos2 = text.find(s);
    while (pos2 != std::string::npos) {
        result.push_back(std::string(text.substr(pos1, pos2 - pos1)));
        pos1 = pos2 + s.size();
        pos2 = text.find(s, pos1);
    }
    result.push_back(std::string(text.substr(pos1)));
    return result;
}

void remove(std::string &text, std::string_view s)
{
    std::string::size_type pos = std::string::npos;
    while ((pos = text.find(s)) != std::string::npos) {
        text.erase(pos, s.length());
    }
}

void replace(std::string &text, std::string_view s, std::string_view r)
{
    std::string::size_type pos = std::string::npos;
    while ((pos = text.find(s)) != std::string::npos) {
        text.replace(pos, s.length(), r);
    }
}

std::string trimmed(std::string_view text)
{
    std::string str(text);
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
                  return !std::isspace(ch);
              }));
    str.erase(std::find_if(str.rbegin(),
                           str.rend(),
                           [](unsigned char ch) { return !std::isspace(ch); })
                  .base(),
              str.end());
    return str;
}

std::string toLower(std::string_view text)
{
    std::string str(text);
    for (std::string::size_type i = 0; i < str.length(); i++) {
        str[i] = std::tolower(str[i]);
    }
    return str;
}

std::string toUpper(std::string_view text)
{
    std::string str(text);
    for (std::string::size_type i = 0; i < str.length(); i++) {
        str[i] = std::toupper(str[i]);
    }
    return str;
}

bool startsWith(std::string_view text, std::string_view str)
{
    return text.find(str) == 0;
}

bool endsWith(std::string_view text, std::string_view str)
{
    return text.rfind(str) == text.length() - str.length();
}

bool contains(std::string_view text, std::string_view str)
{
    return text.find(str) != std::string::npos;
}

bool isLower(std::string_view text)
{
    for (auto c : text) {
        if (c < 'a' || c > 'z') {
            return false;
        }
    }
    return true;
}

bool isUpper(std::string_view text)
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
