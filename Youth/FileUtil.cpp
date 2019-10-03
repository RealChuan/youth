#include "FileUtil.h"
#include "LogOut.h"

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
    fileName_.clear();
    fileName_ += name;
}

std::string FileUtil::fileName() const
{
    return fileName_;
}

bool FileUtil::open(FileUtil::OpenModel model)
{
    if(fileName_.empty()){
        LOG_WARN << "FileName is empty";
        return false;
    }
    openModel_ = model;
    newDirectory();
    switch (model)
    {
    case Read: fp = ::fopen(fileName_.c_str(), "r");break;
    case Write: fp = ::fopen(fileName_.c_str(), "w");break;
    case ReadAndWrite: fp = ::fopen(fileName_.c_str(), "a+");moveFilePoint(Begin);break;
    case Append: fp = ::fopen(fileName_.c_str(), "a+");break;
    default: return false;
    }
    if(fp == nullptr){
        LOG_WARN << "FileName: " << fileName_ << " open error!";
        perror("file open error");
        return false;
    }
    LOG_INFO << "FileName: " << fileName_ << " open succesed!";
    return true;
}

void FileUtil::close()
{
    if(!isOpen())
        return;
    flushFile();
    if(::fclose(fp) == 0)
        fp = nullptr;
    else{
        LOG_WARN << "FileName: " << fileName_ << "close failed";
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
    LOG_WARN << "FileName: " << fileName_ << " is not open!";
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
    write(str.c_str());
}

void FileUtil::write(const char *ch)
{
    if(isOpen() && checkModel(Write))
        ::fwrite(ch, 1, strlen(ch), fp);
}

void FileUtil::flushFile()
{
    if(isOpen())
        ::fflush(fp);
}

bool FileUtil::checkModel(FileUtil::OpenModel model)
{
    bool state = ((openModel_&model) == model);
    if(!state)
        LOG_WARN << "Wrong mode ! Current Model is : " << openModel_;
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

void FileUtil::newDirectory()
{
    if(fileName_.empty())
        return;
    std::vector<String> fileVec = fileName_.split('/');
    size_t size = fileVec.size() - 1;
    for(size_t i=0; i<size; i++){
        mkDir(fileVec[i]);
        fileVec[i+1] = fileVec[i] + "/" +fileVec[i+1];
    }
}

void FileUtil::mkDir(const std::string &str)
{
    if(access(str.c_str(), F_OK) == -1)
        mkdir(str.c_str(), 0777);
}
