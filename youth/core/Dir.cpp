#include "Dir.h"
#include "String.h"

#include <filesystem>
#include <sys/stat.h>
#include <unistd.h>

namespace youth {

namespace core {

bool Dir::cd(const std::filesystem::path &path)
{
    if (!exists(path)) {
        return false;
    }
    if (std::filesystem::is_directory(path)) {
        m_path = path;
        return true;
    }
    return false;
}

bool Dir::mkdir(const std::string &name)
{
    if (!exists()) {
        return false;
    }
    std::filesystem::path path = m_path / name;
    if (std::filesystem::create_directory(path)) {
        return true;
    }
    return false;
}

bool Dir::mkdirs(const std::filesystem::path &path)
{
    if (std::filesystem::exists(path)) {
        return true;
    }
    return std::filesystem::create_directories(path);
}

bool Dir::mkdirs(const std::filesystem::path &path, std::filesystem::perms perms)
{
    if (!mkdirs(path)) {
        return false;
    }
    std::filesystem::permissions(path, perms);
    return std::filesystem::status(path).permissions() == perms;
}

bool Dir::rmdir(const std::filesystem::path &path)
{
    if (std::filesystem::is_directory(path)) {
        return std::filesystem::remove_all(path);
    }
    return false;
}

bool Dir::matchPath(const std::filesystem::path &path, const NameFilterList &namefilters)
{
    for (const auto &name : namefilters) {
        if (matchName(path, name)) {
            return true;
        }
    }
    return false;
}

} // namespace core

} // namespace youth
