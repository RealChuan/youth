#include "FileInfo.hpp"
#include "Dir.h"

namespace youth {

namespace core {

FileInfo::FileInfo(const Dir &dir, const std::string &name)
    : m_path(dir.absolutePath() / name)
{}

Dir FileInfo::absoluteDir() const
{
    return Dir(absolutePath());
}

std::filesystem::path FileInfo::symlinkTarget() const
{
    try {
        return std::filesystem::read_symlink(m_path);
    } catch (const std::filesystem::filesystem_error &) {
        return std::filesystem::path();
    } catch (const std::exception &) {
        return std::filesystem::path();
    }
}

bool FileInfo::isHidden() const
{
    auto name = m_path.filename().string();
    if (name.empty()) {
        return false;
    }
    return name[0] == '.';
}

DateTime FileInfo::lastModified() const
{
    auto time = std::filesystem::last_write_time(m_path);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time.time_since_epoch());
    return DateTime(duration.count());
}

bool FileInfo::makeAbsolute()
{
    if (isAbsolute()) {
        return true;
    }
    m_path = std::filesystem::absolute(m_path);
    return isAbsolute();
}

bool FileInfo::makeAbsolute(const std::filesystem::path &path, std::filesystem::path &absolutePath)
{
    if (path.is_absolute()) {
        absolutePath = path;
        return true;
    }
    absolutePath = std::filesystem::absolute(path);
    return absolutePath.is_absolute();
}

} // namespace core

} // namespace youth
