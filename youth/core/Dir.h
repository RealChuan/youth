#ifndef DIR_H
#define DIR_H

#include <filesystem>
#include <string>
#include <vector>

#include "Object.h"

namespace youth {

namespace core {

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
        FilterType_All,
        FilterType_Dir,
        FilterType_File,
        FilterType_Hidden,
        FilterType_System,
        FilterType_NoDot,
        FilterType_NoDotDot,
        FilterType_NoDotAndDotDot
    };

    using NameFilterList = std::vector<std::string>;

    explicit Dir(const std::filesystem::path &path,
                 const NameFilterList &nameFilters = {},
                 SortType sortType = SortType_None,
                 FilterType filterType = FilterType_All)
        : m_path(path)
        , m_nameFilters(nameFilters)
        , m_sortType(sortType)
        , m_filterType(filterType)
    {}

    Dir(const Dir &other)
        : m_path(other.m_path)
        , m_nameFilters(other.m_nameFilters)
        , m_sortType(other.m_sortType)
        , m_filterType(other.m_filterType)
    {}

    Dir &operator=(const Dir &other)
    {
        if (this != &other) {
            m_path = other.m_path;
            m_nameFilters = other.m_nameFilters;
            m_sortType = other.m_sortType;
            m_filterType = other.m_filterType;
        }
        return *this;
    }

    Dir &operator=(Dir &&other)
    {
        if (this != &other) {
            m_path = std::move(other.m_path);
            m_nameFilters = std::move(other.m_nameFilters);
            m_sortType = other.m_sortType;
            m_filterType = other.m_filterType;
        }
        return *this;
    }

    ~Dir() = default;

    bool operator==(const Dir &other) const
    {
        return m_path == other.m_path && m_nameFilters == other.m_nameFilters
               && m_sortType == other.m_sortType && m_filterType == other.m_filterType;
    }

    bool operator!=(const Dir &other) const { return !operator==(other); }

    void swap(Dir &other)
    {
        if (this != &other) {
            std::swap(m_path, other.m_path);
            std::swap(m_nameFilters, other.m_nameFilters);
            std::swap(m_sortType, other.m_sortType);
            std::swap(m_filterType, other.m_filterType);
        }
    }

    NameFilterList nameFilters() const { return m_nameFilters; }
    void setNameFilters(const NameFilterList &nameFilters) { m_nameFilters = nameFilters; }

    SortType sortType() const { return m_sortType; }
    void setSortType(SortType sortType) { m_sortType = sortType; }

    FilterType filterType() const { return m_filterType; }
    void setFilterType(FilterType filterType) { m_filterType = filterType; }

    std::filesystem::path path() const { return m_path; }
    void setPath(const std::filesystem::path &path) { m_path = path; }

    std::filesystem::path absoluteFilePath(const std::filesystem::path &filepath) const
    {
        if (exists()) {
            return m_path / filepath;
        }
        return filepath;
    }

    std::filesystem::path absolutePath() { return m_path; }
    std::filesystem::path canonicalPath() { return std::filesystem::canonical(m_path); }
    bool cd(const std::filesystem::path &path);
    bool cdup() { return cd(m_path.parent_path()); }
    std::string dirname() { return m_path.filename().string(); }

    bool exists() const { return std::filesystem::exists(m_path); }

    bool mkdir(const std::string &name);

    static bool mkdirs(const std::filesystem::path &path);
    static bool mkdirs(const std::filesystem::path &path, std::filesystem::perms perms);
    static bool rmdir(const std::filesystem::path &path);

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
    NameFilterList m_nameFilters;
    SortType m_sortType = SortType_None;
    FilterType m_filterType = FilterType_All;
};

} // namespace core

} // namespace youth

#endif // DIR_H
