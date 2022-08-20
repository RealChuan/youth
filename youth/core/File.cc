#include "File.hpp"

#include <cstring>

namespace youth {

namespace core {

bool File::open(const std::filesystem::path &filename, OpenMode mode)
{
    close();
    m_filePath = filename;
    m_openMode = mode;
    m_fstream.open(m_filePath, static_cast<std::ios_base::openmode>(m_openMode));
    return m_fstream.is_open();
}

void File::close()
{
    if (m_fstream.is_open()) {
        m_fstream.flush();
        m_fstream.close();
    }
}

void File::flush()
{
    if (m_fstream.is_open()) {
        m_fstream.flush();
    }
}

std::string File::readLine()
{
    std::string line;
    if (!isOpen() || !checkMode(ReadOnly)) {
        return line;
    }
    std::getline(m_fstream, line);
    return line;
}

std::string File::readAll()
{
    std::string all;
    if (!isOpen() || !checkMode(ReadOnly)) {
        return all;
    }
    std::string line;
    while (!atEnd()) {
        std::getline(m_fstream, line);
        if (line.empty()) {
            break;
        }
        all += line; // need to add "\n" ?
        line.clear();
    }
    return all;
}

void File::write(const char *str, int len)
{
    if (!isOpen() || !checkMode(WriteOnly)) {
        return;
    }
    m_fstream.write(str, len);
}

void File::seek(int offset)
{
    if (isOpen()) {
        m_fstream.seekp(offset);
    }
}

void File::seek(SeekType type, int offset)
{
    if (isOpen()) {
        m_fstream.seekp(offset, static_cast<std::ios_base::seekdir>(type));
    }
}

std::streampos File::tell()
{
    if (isOpen()) {
        return m_fstream.tellp();
    }
    return -1;
}

bool File::atEnd()
{
    if (!isOpen()) {
        return true;
    }

    return m_fstream.peek() == EOF;
}

int64_t File::size() const
{
    int64_t s = 0;
    try {
        s = std::filesystem::file_size(m_filePath);
    } catch (std::filesystem::filesystem_error &e) {
        fprintf(stderr, "%s\n", e.what());
    }
    return s;
}

bool File::remove()
{
    if (!isOpen()) {
        return false;
    }
    return remove(m_filePath);
}

std::string File::errorString() const
{
    std::string err;
    auto rdstate = m_fstream.rdstate();
    if (rdstate & std::ios::failbit) {
        err = "failbit ";
    }
    if (rdstate & std::ios::eofbit) {
        err += "eofbit ";
    }
    if (rdstate & std::ios::badbit) {
        err += "badbit ";
    }
    if (rdstate & std::ios::goodbit) {
        err += "goodbit ";
    }
    return err;
}

bool File::copy(const std::filesystem::path &src, const std::filesystem::path &dst)
{
    bool ret = false;
    std::error_code error;
    try {
        ret = std::filesystem::copy_file(src, dst, error);
    } catch (std::filesystem::filesystem_error &e) {
        fprintf(stderr, "%s\n", e.what());
    }
    if (!ret) {
        fprintf(stderr, "%s\n", error.message().c_str());
    }
    return ret;
}

bool File::checkMode(OpenMode mode)
{
    bool state = (m_openMode & mode) == mode;
    if (!state) {
        fprintf(stderr, "File::checkMode: open mode error\n");
    }
    return state;
}

} // namespace core

} // namespace youth
