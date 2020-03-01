#include "FileUtil.h"
#include "Dir.h"

#include <unistd.h>
#include <sys/stat.h>

#define MAX_LINE 1024

using namespace youth;

FileUtil::FileUtil()
    :fp()
{

}

FileUtil::FileUtil(const std::string& filename)
    :fp()
    ,fileName_(filename)
{

}

FileUtil::~FileUtil()
{
    close();
}

void FileUtil::setFileName(const std::string& name)
{
    fileName_ = name;
}

void FileUtil::setFileName(const char *name)
{
    fileName_ = name;
}

std::string FileUtil::fileName() const
{
    return fileName_;
}

bool FileUtil::open(FileUtil::OpenModel model)
{
    //close();
    if(fileName_.empty()){
        printf("FileName is empty!\n");
        return false;
    }
    openModel_ = model;
    Dir::newDirectory(fileName_);
    switch (model)
    {
    case Read: fp = ::fopen(fileName_.c_str(), "r");break;
    case Write: fp = ::fopen(fileName_.c_str(), "w");break;
    case ReadAndWrite: fp = ::fopen(fileName_.c_str(), "a+");break;
    case Append: fp = ::fopen(fileName_.c_str(), "a+");break;
    default: return false;
    }
    if(fp == nullptr){
        fprintf(stderr, "FileName: %s open error!\n", fileName_.c_str());
        perror("file open error");
        return false;
    }
    printf("FileName: %s open succesed!\n", fileName_.c_str());
    return true;
}

void FileUtil::close()
{
    if(!isOpen())
        return;
    flushFile();
    if(::fclose(fp) == 0){
        fp = nullptr;
        printf("FileName: %s close succesed!\n", fileName_.c_str());
    }else{
        fprintf(stderr, "FileName: %s close failed!\n", fileName_.c_str());
        perror("file close error");
    }
}

void FileUtil::setOpenModel(OpenModel model)
{
    openModel_ = model;
}

FileUtil::OpenModel FileUtil::openModel() const
{
    return openModel_;
}

bool FileUtil::isOpen()
{
    if(fp != nullptr)
        return true;
    fprintf(stderr, "FileName: %s is not open!\n", fileName_.c_str());
    return false;
}

std::string FileUtil::readLine()
{
    std::string str;
    if(!isOpen() || !checkModel(Read)){
        return str;
    }
    char buf[MAX_LINE];
    //int len;    /*行字符个数*/
    if(fgets(buf, MAX_LINE, fp) != nullptr){
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
    do{
        s = readLine();
        str += s;
        s.clear();
    }while(!s.empty());
    return str;
}

void FileUtil::write(const std::string &str)
{
    write(str.c_str(), str.size());
}

void FileUtil::write(const char *ch, int len)
{
    if(isOpen() && checkModel(Write)){
        ::fwrite(ch, 1, static_cast<size_t>(len), fp);
        writeBytes_ += len;
    }
}

void FileUtil::flushFile()
{
    if(isOpen())
        ::fflush(fp);
}

off_t FileUtil::writeBytes() const
{
    return writeBytes_;
}

bool FileUtil::checkModel(FileUtil::OpenModel model)
{
    bool state = ((openModel_&model) == model);
    if(!state)
      fprintf(stderr, "Wrong mode ! Current Model is %d !\n", openModel_);
    return state;
}

void FileUtil::moveFilePoint(FileUtil::Seek seek)
{
    if(!isOpen())
        return;
    switch (seek) {
    case Begin : fseek(fp, 0, SEEK_SET);break;
    case Current : fseek(fp, 0, SEEK_CUR);break;
    case End : fseek(fp, 0, SEEK_END);break;
    default : break;
    }
}
