#ifndef FILEUTIL_H
#define FILEUTIL_H

#include "Object.h"
#include "String.h"

namespace youth {

namespace core {
//  thread not safe
class FileUtil : noncopyable
{
public:
    enum OpenModel {
        Read = 0x01,
        Write = 0x02,
        ReadAndWrite = Read | Write,
        Append = 0x04 | ReadAndWrite
    };

    enum Seek { Begin, Current, End };

    FileUtil()
        : m_filePtr()
    {}
    FileUtil(const std::string &filename)
        : m_filePtr()
        , m_fileName(filename)
    {}
    ~FileUtil() { close(); }

    void setFileName(const std::string &name) { m_fileName = name; }
    void setFileName(const char *name) { m_fileName = name; }
    std::string fileName() const { return m_fileName; }

    bool open(OpenModel);
    void close();

    void setOpenModel(OpenModel model) { m_openModel = model; }
    OpenModel openModel() const { return m_openModel; }

    bool isOpen();

    std::string readLine();
    std::string readAll();

    void write(const std::string &str) { write(str.c_str(), int(str.size())); }

    void write(const char *, int len);

    void flushFile();

    off_t writeBytes() const { return m_writeBytes; }

    void moveFilePoint(Seek);

    //int size();

private:
    inline bool checkModel(OpenModel);

    FILE *m_filePtr;
    std::string m_fileName;
    OpenModel m_openModel;
    off_t m_writeBytes = 0;
};

} // namespace core

} // namespace youth

#endif // FILEUTIL_H
