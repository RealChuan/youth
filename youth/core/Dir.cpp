#include "Dir.h"
#include "FileInfo.hpp"
#include "String.h"

#include <filesystem>
#include <sys/stat.h>
#include <unistd.h>

namespace youth {

namespace core {

bool Contains(const std::string &filename, const Dir::NameFilterList &nameFilters)
{
    for (auto &filter : nameFilters) {
        if (filename.find(filter) != std::string::npos) {
            return true;
        }
    }
    return false;
}

bool isFilterType(const std::filesystem::path &path, Dir::FilterType filterType)
{
    bool isAll = (filterType & Dir::FilterType::FilterType_All) == Dir::FilterType::FilterType_All;
    if (isAll) {
        return true;
    }
    bool isDir = (filterType & Dir::FilterType::FilterType_Dir) == Dir::FilterType::FilterType_Dir;
    bool isFile = (filterType & Dir::FilterType::FilterType_File)
                  == Dir::FilterType::FilterType_File;
    bool isHidden = (filterType & Dir::FilterType::FilterType_Hidden)
                    == Dir::FilterType::FilterType_Hidden;
    bool isNoDot = (filterType & Dir::FilterType::FilterType_NoDot)
                   == Dir::FilterType::FilterType_NoDot;
    bool isNoDotDot = (filterType & Dir::FilterType::FilterType_NoDotDot)
                      == Dir::FilterType::FilterType_NoDotDot;
    auto filename = path.filename().string();
    if (isDir && std::filesystem::is_directory(path)) {
        if (isNoDotDot && filename.size() == 2 && filename[0] == '.' && filename[1] == '.') {
            return false;
        }
        if (isNoDot && filename.size() == 1 && filename[0] == '.') {
            return false;
        }
        if (isHidden && !filename.empty() && filename[0] == '.') {
            return true;
        }
    }
    if (isFile && std::filesystem::is_regular_file(path)) {
        if (isNoDotDot && filename.size() == 2 && filename[0] == '.' && filename[1] == '.') {
            return false;
        }
        if (isNoDot && filename.size() == 1 && filename[0] == '.') {
            return false;
        }
        if (isHidden && !filename.empty() && filename[0] == '.') {
            return true;
        }
    }
    return false;
}

FileInfoList Dir::entryInfoList(const NameFilterList &nameFilters,
                                FilterType filterType,
                                SortType sortType) const
{
    FileInfoList list;
    if (!exists(m_path)) {
        return list;
    }

    for (auto &iter : std::filesystem::directory_iterator(m_path)) {
        auto path = iter.path();
        auto filename = path.string();
        if (Contains(filename, nameFilters)) {
            continue;
        }
        if (!isFilterType(path, filterType)) {
            continue;
        }
        list.emplace_back(iter.path());
    }

    return list;
}

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

bool Dir::rmdir(const std::string &name)
{
    if (!exists()) {
        return false;
    }
    std::filesystem::path path = m_path / name;
    if (std::filesystem::remove_all(path)) {
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

bool Dir::rmdirs(const std::filesystem::path &path)
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
