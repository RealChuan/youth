#pragma once

#include <string>
#include <vector>

namespace youth {

namespace core {

namespace string {

std::vector<std::string> split(const std::string &text, const std::string &s);

void remove(std::string &text, const std::string &s);

void replace(std::string &text, const std::string &s, const std::string &r);

std::string trimmed(const std::string &text);

std::string toLower(const std::string &text);

std::string toUpper(const std::string &text);

bool startsWith(const std::string &text, const std::string &str);

bool endsWith(const std::string &text, const std::string &str);

bool contains(const std::string &text, const std::string &str);

bool isLower(const std::string &text);

bool isUpper(const std::string &text);

} // namespace string

} // namespace core

} // namespace youth
