#pragma once

#include "Object.h"

#include <filesystem>
#include <fstream>

namespace youth {

namespace core {

class File : noncopyable
{
public:
    enum OpenMode {
        ReadOnly = std::ios::in,
        WriteOnly = std::ios::out,
        ReadAndWrite = ReadOnly | WriteOnly,
        Append = std::ios::app | ReadAndWrite,
        Truncate = std::ios::trunc,
        Binary = std::ios::binary
    };

    enum SeekType { Begin = std::ios::beg, Current = std::ios::cur, End = std::ios::end };

    explicit File() {}
    explicit File(const std::filesystem::path &filename)
        : m_filePath(filename)
    {}
    ~File() { close(); }

    bool open(const std::filesystem::path &filename, OpenMode mode);
    bool open(OpenMode mode) { return open(m_filePath, mode); }
    bool open() { return open(m_filePath, m_openMode); }
    void close();
    void flush();

    void setFileName(const std::filesystem::path &filename) { m_filePath = filename; }

    std::filesystem::path filesystemFileName() const { return m_filePath; }
    std::string fileName() const { return m_filePath.string(); }

    void setOpenMode(OpenMode mode) { m_openMode = mode; }
    OpenMode openMode() const { return m_openMode; }

    bool isOpen() const { return m_fstream.is_open(); }

    std::string readLine();
    std::string readAll();

    void write(const std::string &str) { write(str.c_str(), str.size()); }
    void write(const char *str, int len);

    void seek(int offset);
    void seek(SeekType type, int offset = 0);

    std::streampos tell();

    bool atEnd();

    int64_t size() const;

    int error() const { return m_fstream.rdstate(); }
    std::string errorString() const;

    bool copyTo(const std::filesystem::path &dest) { return copy(m_filePath, dest); }

    bool exists() { return std::filesystem::exists(m_filePath); }

    bool renameTo(const std::filesystem::path &dest) { return rename(m_filePath, dest); }

    std::filesystem::path filesystemSymLinkTarget() { return filesystemSymLinkTarget(m_filePath); }

    bool link(const std::filesystem::path &linkName) { return link(m_filePath, linkName); }

    static bool copy(const std::filesystem::path &src, const std::filesystem::path &dst);

    static bool exists(const std::filesystem::path &path) { return std::filesystem::exists(path); }

    static bool remove(const std::filesystem::path &path) { return std::filesystem::remove(path); }

    static bool rename(const std::filesystem::path &src, const std::filesystem::path &dst)
    {
        std::filesystem::rename(src, dst);
        return exists(dst);
    }

    static std::filesystem::path filesystemSymLinkTarget(const std::filesystem::path &fileName)
    {
        return std::filesystem::read_symlink(fileName);
    }

    static bool link(const std::filesystem::path &fileName, const std::filesystem::path &linkName)
    {
        std::error_code code;
        std::filesystem::create_symlink(fileName, linkName, code);
        return code.value() == 0;
    }

private:
    inline bool checkMode(OpenMode);

    OpenMode m_openMode = ReadOnly;
    std::filesystem::path m_filePath;
    std::fstream m_fstream;
};

} // namespace core

} // namespace youth
