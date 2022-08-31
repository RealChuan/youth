#include "Dir.h"
#include "FileInfo.hpp"
#include "StringFunction.hpp"

#include <algorithm>

namespace youth {

namespace core {

std::filesystem::path Dir::absolutePath() const
{
    std::filesystem::path absolutePath;
    FileInfo::makeAbsolute(m_path, absolutePath);
    return absolutePath;
}

bool Contains(std::string_view filename, const Dir::NameFilterList &nameFilters)
{
    for (const auto &nameFilter : nameFilters) {
        if (string::contains(filename, nameFilter)) {
            return true;
        }
    }
    return false;
}

bool isFilterType(const std::filesystem::path &path, Dir::FilterType filterType)
{
    auto filename = path.filename().string();
    bool isHidden = filename[0] == '.';
    bool isDir = std::filesystem::is_directory(path);
    bool isFile = std::filesystem::is_regular_file(path);
    bool isNoDot = filename == ".";
    bool isNoDotDot = filename == "..";

    if (filterType & Dir::FilterType_NoDot) {
        if (isNoDot) {
            return false;
        }
    }
    if (filterType & Dir::FilterType_NoDotDot) {
        if (isNoDotDot) {
            return false;
        }
    }
    if (filterType & Dir::FilterType_Dir) {
        if (isDir) {
            if (filterType & Dir::FilterType_Hidden) {
                if (isHidden) {
                    return true;
                }
            } else {
                return true;
            }
        }
    }
    if (filterType & Dir::FilterType_File) {
        if (isFile) {
            if (filterType & Dir::FilterType_Hidden) {
                if (isHidden) {
                    return true;
                }
            } else {
                return true;
            }
        }
    }

    return false;
}

void sort(FileInfoList &list, Dir::SortType sortType)
{
    if (sortType == Dir::SortType_None) {
        return;
    }
    if (sortType == Dir::SortType_Name) {
        std::sort(list.begin(), list.end(), [](const FileInfo &a, const FileInfo &b) {
            return a.fileName() < b.fileName();
        });
    } else if (sortType == Dir::SortType_Time) {
        std::sort(list.begin(), list.end(), [](const FileInfo &a, const FileInfo &b) {
            return a.lastModified() < b.lastModified();
        });
    } else if (sortType == Dir::SortType_Size) {
        std::sort(list.begin(), list.end(), [](const FileInfo &a, const FileInfo &b) {
            return a.size() < b.size();
        });
    } else if (sortType == Dir::SortType_DirsFirst) {
        std::sort(list.begin(), list.end(), [](const FileInfo &a, const FileInfo &b) {
            return a.isDir() < b.isDir();
        });
    } else if (sortType == Dir::SortType_DirsLast) {
        std::sort(list.begin(), list.end(), [](const FileInfo &a, const FileInfo &b) {
            return a.isDir() > b.isDir();
        });
    }
}

FileInfoList Dir::entryInfoList(const NameFilterList &nameFilters,
                                FilterType filterType,
                                SortType sortType) const
{
    FileInfoList list;
    if (!exists(m_path)) {
        return list;
    }
    for (const auto &iter : std::filesystem::directory_iterator(m_path)) {
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
    sort(list, sortType);
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

bool Dir::mkdir(std::string_view name)
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

bool Dir::rmdir(std::string_view name)
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

bool Dir::removeFile(std::string_view name)
{
    return removeFile(m_path, name);
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

bool Dir::removeFile(const std::filesystem::path &path, std::string_view filename)
{
    if (!std::filesystem::exists(path)) {
        return false;
    }
    if (!std::filesystem::is_directory(path)) {
        return false;
    }
    std::filesystem::path filepath = path / filename;
    if (!std::filesystem::exists(filepath)) {
        return false;
    }
    if (!std::filesystem::is_regular_file(filepath)) {
        return false;
    }
    return std::filesystem::remove(filepath);
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
