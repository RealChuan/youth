#pragma once

#include "DateTime.hpp"

#include <filesystem>

namespace youth {

namespace core {

class Dir;

class FileInfo : copyable
{
public:
    FileInfo() = default;
    FileInfo(const Dir &dir, const std::string &name);
    FileInfo(const std::filesystem::path &path)
        : m_path(path)
    {}
    FileInfo(const FileInfo &other)
        : m_path(other.m_path)
    {}

    FileInfo &operator=(const FileInfo &other)
    {
        if (this != &other) {
            m_path = other.m_path;
        }
        return *this;
    }
    FileInfo &operator=(FileInfo &&other)
    {
        if (this != &other) {
            m_path = std::move(other.m_path);
        }
        return *this;
    }

    ~FileInfo() = default;

    bool operator==(const FileInfo &other) const { return m_path == other.m_path; }
    bool operator!=(const FileInfo &other) const { return m_path != other.m_path; }

    void swap(FileInfo &other) { m_path.swap(other.m_path); }

    Dir absoluteDir() const;
    std::filesystem::path absoluteFilePath() const { return m_path; }
    std::filesystem::path absolutePath() const { return m_path.parent_path(); }
    std::filesystem::path canonicalFilePath() const { return std::filesystem::canonical(m_path); }
    std::filesystem::path canonicalPath() const
    {
        return std::filesystem::canonical(m_path.parent_path());
    }
    std::filesystem::path symlinkTarget() const { return std::filesystem::read_symlink(m_path); }

    std::string fileName() const { return m_path.filename().string(); }
    std::string bundleName() const { return m_path.stem().string(); }
    std::string suffix() const { return m_path.extension().string(); }

    bool isAbsolute() const { return m_path.is_absolute(); }
    bool isBlockFile() const { return std::filesystem::is_block_file(m_path); }
    bool isBundle() const { return m_path.extension() == ".app"; }
    bool isCharacterFile() const { return std::filesystem::is_character_file(m_path); }
    bool isDir() const { return std::filesystem::is_directory(m_path); }
    bool isEmptyFileOrDir() const { return std::filesystem::is_empty(m_path); }
    bool isFifo() const { return std::filesystem::is_fifo(m_path); }
    bool isFile() const { return std::filesystem::is_regular_file(m_path); }
    bool isHidden() const;
    bool isOther() const { return std::filesystem::is_other(m_path); }
    bool isRelative() const { return m_path.is_relative(); }
    bool isRoot() const { return m_path.is_absolute() && m_path.root_path() == m_path; }
    bool isShortcut() const { return m_path.extension() == ".lnk"; }
    bool isSocket() const { return std::filesystem::is_socket(m_path); }
    bool isSymLink() const { return std::filesystem::is_symlink(m_path); }

    DateTime lastModified() const;
    bool makeAbsolute();

    void setFile(const std::filesystem::path &path) { m_path = path; }
    int64_t size() const { return std::filesystem::file_size(m_path); }

    static bool exists(const std::filesystem::path &path) { return std::filesystem::exists(path); }

private:
    std::filesystem::path m_path;
};

} // namespace core

} // namespace youth
