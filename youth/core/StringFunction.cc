#include "StringFunction.hpp"

#include <algorithm>
#include <string>

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

void remove(std::string &text, const std::string &s)
{
    std::string::size_type pos = std::string::npos;
    while ((pos = text.find(s)) != std::string::npos) {
        text.erase(pos, s.length());
    }
}

void replace(std::string &text, const std::string &s, const std::string &r)
{
    std::string::size_type pos = std::string::npos;
    while ((pos = text.find(s)) != std::string::npos) {
        text.replace(pos, s.length(), r);
    }
}

std::string trimmed(const std::string &text)
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

std::string toLower(const std::string &text)
{
    std::string str(text);
    for (std::string::size_type i = 0; i < str.length(); i++) {
        str[i] = std::tolower(str[i]);
    }
    return str;
}

std::string toUpper(const std::string &text)
{
    std::string str(text);
    for (std::string::size_type i = 0; i < str.length(); i++) {
        str[i] = std::toupper(str[i]);
    }
    return str;
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
