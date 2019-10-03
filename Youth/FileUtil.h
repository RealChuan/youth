#ifndef FILEUTIL_H
#define FILEUTIL_H

#include "String.h"

namespace youth {

//  thread not safe

class FileUtil
{
public:
    enum OpenModel{
        Read = 0x01,
        Write = 0x02,
        ReadAndWrite = Read|Write,
        Append = 0x04|ReadAndWrite
    };

    enum Seek{
        Begin,
        Current,
        End
    };

    FileUtil();
    FileUtil(const std::string&);
    FileUtil(const String&);
    ~FileUtil();

    void setFileName(const std::string&);
    void setFileName(const String&);
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
    void write(const char*);

    void flushFile();

    //int size();

private:
    inline bool checkModel(OpenModel);
    inline void moveFilePoint(Seek);
    inline void newDirectory();
    inline void mkDir(const std::string&);

    FILE *fp;
    String fileName_;
    OpenModel openModel_;
};

}

#endif // FILEUTIL_H
