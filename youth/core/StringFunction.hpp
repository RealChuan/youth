#pragma once

#include <string>
#include <vector>

namespace youth {

namespace core {

namespace string {

std::vector<std::string> split(std::string_view text, std::string_view s);

void remove(std::string &text, std::string_view s);

void replace(std::string &text, std::string_view s, std::string_view r);

std::string trimmed(std::string_view text);

std::string toLower(std::string_view text);

std::string toUpper(std::string_view text);

bool startsWith(std::string_view text, std::string_view str);

bool endsWith(std::string_view text, std::string_view str);

bool contains(std::string_view text, std::string_view str);

bool isLower(std::string_view text);

bool isUpper(std::string_view text);

} // namespace string

} // namespace core

} // namespace youth
