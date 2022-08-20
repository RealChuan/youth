#ifndef DIR_H
#define DIR_H

#include <filesystem>
#include <string>
#include <vector>

#include "Object.h"

namespace youth {

namespace core {

class FileInfo;
using FileInfoList = std::vector<FileInfo>;

class Dir : copyable
{
public:
    enum SortType {
        SortType_None,
        SortType_Name,
        SortType_Time,
        SortType_Size,
        SortType_DirsFirst,
        SortType_DirsLast,
    };
    enum FilterType {
        FilterType_Dir = 1,
        FilterType_File = 2,
        FilterType_Hidden = 4,
        FilterType_NoDot = 8,
        FilterType_NoDotDot = 16,
        FilterType_NoDotAndDotDot = FilterType_NoDot | FilterType_NoDotDot,
        FilterType_All = FilterType_Dir | FilterType_File
    };

    using NameFilterList = std::vector<std::string>;

    explicit Dir(const std::filesystem::path &path)
        : m_path(path)
    {}

    Dir(const Dir &other)
        : m_path(other.m_path)
    {}

    Dir &operator=(const Dir &other)
    {
        if (this != &other) {
            m_path = other.m_path;
        }
        return *this;
    }

    Dir &operator=(Dir &&other)
    {
        if (this != &other) {
            m_path = std::move(other.m_path);
        }
        return *this;
    }

    ~Dir() = default;

    bool operator==(const Dir &other) const { return m_path == other.m_path; }

    bool operator!=(const Dir &other) const { return !operator==(other); }

    void swap(Dir &other)
    {
        if (this != &other) {
            std::swap(m_path, other.m_path);
        }
    }

    std::filesystem::path path() const { return m_path; }
    void setPath(const std::filesystem::path &path) { m_path = path; }

    std::filesystem::path absoluteFilePath(const std::string &filename) const
    {
        return absolutePath() / filename;
    }
    std::filesystem::path absolutePath() const;
    std::filesystem::path canonicalPath() const { return std::filesystem::canonical(m_path); }
    std::string dirname() { return m_path.filename().string(); }

    FileInfoList entryInfoList(const NameFilterList &nameFilter,
                               FilterType filterType = FilterType_All,
                               SortType sortType = SortType_None) const;

    bool cd(const std::filesystem::path &path);
    bool cdup() { return cd(m_path.parent_path()); }

    bool exists() const { return std::filesystem::exists(m_path); }

    bool mkdir(const std::string &name);
    bool rmdir(const std::string &name);
    bool removeFile(const std::string &name);

    static bool mkdirs(const std::filesystem::path &path);
    static bool mkdirs(const std::filesystem::path &path, std::filesystem::perms perms);
    static bool rmdirs(const std::filesystem::path &path);
    static bool removeFile(const std::filesystem::path &path, const std::string &filename);

    static Dir Current() { return Dir(std::filesystem::current_path()); }
    static Dir temp() { return Dir(std::filesystem::temp_directory_path()); }
    static bool isAbsolute(const std::filesystem::path &path) { return path.is_absolute(); }
    static bool isRelative(const std::filesystem::path &path) { return path.is_relative(); }

    static std::filesystem::path fromNativeSeparators(const std::string &path)
    {
        return std::filesystem::path(path).make_preferred();
    }
    static std::string toNativeSeparators(std::filesystem::path &path) { return path.native(); }

    static bool matchName(const std::filesystem::path &path, const std::string &namefilter)
    {
        return path.filename().string() == namefilter;
    }
    static bool matchPath(const std::filesystem::path &path, const NameFilterList &namefilter);
    static bool setCurrent(const std::filesystem::path &path)
    {
        std::filesystem::current_path(path);
        return std::filesystem::current_path() == path;
    }
    static bool exists(const std::filesystem::path &path) { return std::filesystem::exists(path); }

private:
    std::filesystem::path m_path;
};

} // namespace core

} // namespace youth

#endif // DIR_H
