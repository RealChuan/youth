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

std::string fileBaseName(const std::string &basename);

int excute(const std::string &cmd);

bool kill(pid_t pid);
bool kill(const std::string &name);

pid_t getPidFromName(const std::string &name);

std::string getCwd();

std::vector<std::string> systemEnvironment();

bool setEnvironment(const std::string &name, const std::string &value);
std::string getEnvironment(const std::string &name);

File nullDevice();

} // namespace Process

} // namespace core

} // namespace youth
