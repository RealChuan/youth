#include "FileUtil.h"
#include "Dir.h"

#include <sys/stat.h>
#include <unistd.h>

#define MAX_LINE 1024

namespace youth {

namespace core {

bool FileUtil::open(FileUtil::OpenModel model)
{
    //close();
    if (m_fileName.empty()) {
        printf("FileName is empty!\n");
        return false;
    }
    m_openModel = model;
    Dir::newDirectory(m_fileName);
    switch (model) {
    case Read: m_filePtr = ::fopen(m_fileName.c_str(), "r"); break;
    case Write: m_filePtr = ::fopen(m_fileName.c_str(), "w"); break;
    case ReadAndWrite: m_filePtr = ::fopen(m_fileName.c_str(), "a+"); break;
    case Append: m_filePtr = ::fopen(m_fileName.c_str(), "a+"); break;
    default: return false;
    }
    if (m_filePtr == nullptr) {
        fprintf(stderr, "FileName: %s open error!\n", m_fileName.c_str());
        perror("file open error");
        return false;
    }
    printf("FileName: %s open succesed!\n", m_fileName.c_str());
    return true;
}

void FileUtil::close()
{
    if (!isOpen())
        return;
    flushFile();
    if (::fclose(m_filePtr) == 0) {
        m_filePtr = nullptr;
        printf("FileName: %s close succesed!\n", m_fileName.c_str());
    } else {
        fprintf(stderr, "FileName: %s close failed!\n", m_fileName.c_str());
        perror("file close error");
    }
}

bool FileUtil::isOpen()
{
    if (m_filePtr != nullptr)
        return true;
    fprintf(stderr, "FileName: %s is not open!\n", m_fileName.c_str());
    return false;
}

std::string FileUtil::readLine()
{
    std::string str;
    if (!isOpen() || !checkModel(Read)) {
        return str;
    }
    char buf[MAX_LINE];
    //int len;    /*行字符个数*/
    if (fgets(buf, MAX_LINE, m_filePtr) != nullptr) {
        //len = strlen(buf);
        //buf[len-1] = '\0';  /*去掉换行符*/
        str = buf;
    }
    return str;
}

std::string FileUtil::readAll()
{
    std::string str;
    std::string s;
    do {
        s = readLine();
        str += s;
        s.clear();
    } while (!s.empty());
    return str;
}

void FileUtil::write(const char *ch, int len)
{
    if (isOpen() && checkModel(Write)) {
        ::fwrite(ch, 1, static_cast<size_t>(len), m_filePtr);
        m_writeBytes += len;
    }
}

void FileUtil::flushFile()
{
    if (isOpen())
        ::fflush(m_filePtr);
}

bool FileUtil::checkModel(FileUtil::OpenModel model)
{
    bool state = ((m_openModel & model) == model);
    if (!state) {
        fprintf(stderr, "Wrong mode ! Current Model is %d !\n", m_openModel);
    }
    return state;
}

void FileUtil::moveFilePoint(FileUtil::Seek seek)
{
    if (!isOpen()) {
        return;
    }
    switch (seek) {
    case Begin: fseek(m_filePtr, 0, SEEK_SET); break;
    case Current: fseek(m_filePtr, 0, SEEK_CUR); break;
    case End: fseek(m_filePtr, 0, SEEK_END); break;
    default: break;
    }
}

} // namespace core

} // namespace youth
