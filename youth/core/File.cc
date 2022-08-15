#include "File.hpp"

#include <cstring>

namespace youth {

namespace core {

bool File::open(const char *filename, OpenMode mode)
{
    close();
    m_filename = filename;
    m_fstream.open(filename, static_cast<std::ios_base::openmode>(mode));
    return m_fstream.is_open();
}

bool File::open()
{
    return open(m_filename.c_str(), m_openMode);
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
    while (m_fstream.peek() != EOF) {
        std::getline(m_fstream, line);
        if (line.empty()) {
            break;
        }
        all += line;
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

void File::seek(SeekType type, int offset)
{
    if (isOpen()) {
        m_fstream.seekp(offset, static_cast<std::ios_base::seekdir>(type));
    }
}

int64_t File::size() const
{
    std::ifstream ifs(m_filename, std::ios::binary | std::ios::ate);
    return ifs.tellg();
}

std::string File::errorString() const
{
    std::string err;
    if (m_fstream.rdstate() & std::ios::failbit) {
        err = "failbit ";
    }
    if (m_fstream.rdstate() & std::ios::eofbit) {
        err += "eofbit ";
    }
    if (m_fstream.rdstate() & std::ios::badbit) {
        err += "badbit ";
    }
    if (m_fstream.rdstate() & std::ios::goodbit) {
        err += "goodbit ";
    }
    return err;
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