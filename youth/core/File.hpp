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
        : m_filename(filename.string())
    {}
    explicit File(const std::string_view &filename)
        : m_filename(filename)
    {}
    explicit File(const std::string &filename)
        : m_filename(filename)
    {}
    explicit File(const char *filename)
        : m_filename(filename)
    {}
    ~File() { close(); }

    bool open(const std::filesystem::path &filename, OpenMode mode)
    {
        return open(filename.string(), mode);
    }
    bool open(const std::string_view &filename, OpenMode mode = ReadAndWrite)
    {
        return open(filename.data(), mode);
    }
    bool open(const std::string &filename, OpenMode mode = ReadAndWrite)
    {
        return open(filename.c_str(), mode);
    }
    bool open(const char *filename, OpenMode mode = ReadAndWrite);
    bool open();
    void close();
    void flush();

    void setFileName(const std::filesystem::path &filename) { m_filename = filename.string(); }
    void setFileName(const std::string_view &name) { m_filename = name; }
    void setfileName(const std::string &name) { m_filename = name; }
    void setFileName(const char *name) { m_filename = name; }

    std::filesystem::path filesystemFileName() const { return std::filesystem::path(m_filename); }
    std::string fileName() const { return m_filename; }

    void setOpenMode(OpenMode mode) { m_openMode = mode; }
    OpenMode openMode() const { return m_openMode; }

    bool isOpen() const { return m_fstream.is_open(); }

    std::string readLine();
    std::string readAll();

    void write(const std::string_view &str) { write(str.data(), str.size()); }
    void write(const std::string &str) { write(str.c_str(), str.size()); }
    void write(const char *str, int len);

    void seek(SeekType type, int offset = 0);

    int64_t size() const;

    int error() const { return m_fstream.rdstate(); }
    std::string errorString() const;

    bool copyTo(const std::filesystem::path &dest) { return copy(m_filename, dest); }

    bool exists() { return std::filesystem::exists(m_filename); }

    bool renameTo(const std::filesystem::path &dest) { return rename(m_filename, dest); }

    static bool copy(const std::filesystem::path &src, const std::filesystem::path &dst)
    {
        return std::filesystem::copy_file(src, dst);
    }

    static bool exists(const std::filesystem::path &path) { return std::filesystem::exists(path); }

    static bool remove(const std::filesystem::path &path) { return std::filesystem::remove(path); }

    static bool rename(const std::filesystem::path &src, const std::filesystem::path &dst)
    {
        std::filesystem::rename(src, dst);
        return exists(dst);
    }

private:
    inline bool checkMode(OpenMode);

    OpenMode m_openMode = ReadOnly;
    std::string m_filename;
    std::fstream m_fstream;
};

} // namespace core

} // namespace youth
