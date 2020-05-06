#ifndef FILEUTIL_H
#define FILEUTIL_H

#include "String.h"
#include "Object.h"

namespace youth {

//  thread not safe

class FileUtil : public noncopyable
{
public:
    enum OpenModel
    {
        Read = 0x01,
        Write = 0x02,
        ReadAndWrite = Read|Write,
        Append = 0x04|ReadAndWrite
    };

    enum Seek{ Begin, Current, End };

    FileUtil();
    FileUtil(const std::string&);
    ~FileUtil();

    void setFileName(const std::string&);
    void setFileName(const char*);
    std::string fileName() const;

    bool open(OpenModel);
    void close();

    void setOpenModel(OpenModel);
    OpenModel openModel() const;

    bool isOpen();

    std::string readLine();
    std::string readAll();

    void write(const std::string&);
    void write(const char*, int len);

    void flushFile();

    off_t writeBytes() const;

    void moveFilePoint(Seek);

    //int size();

private:
    inline bool checkModel(OpenModel);

    FILE *fp;
    std::string fileName_;
    OpenModel openModel_;
    off_t writeBytes_ = 0;
};

}

#endif // FILEUTIL_H
