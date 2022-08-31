#pragma once

#include <string>
#include <vector>

namespace youth {

namespace core {

class File;

namespace Process {

pid_t getTid();

pid_t getPid();

std::string hostname();

std::string fileBaseName(std::string_view basename);

int excute(std::string_view cmd);

bool kill(pid_t pid);
bool kill(const std::string &name);

pid_t getPidFromName(const std::string &name);

std::string getCwd();

std::vector<std::string> systemEnvironment();

bool setEnvironment(std::string name, std::string_view value);
std::string getEnvironment(std::string_view name);

File nullDevice();

} // namespace Process

} // namespace core

} // namespace youth
