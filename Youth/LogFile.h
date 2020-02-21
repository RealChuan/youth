/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LogFile.h
 * Author: root
 *
 * Created on 2019年8月4日, 上午7:24
 */

#ifndef LOGFILE_H
#define LOGFILE_H

#include <string>

#include "Thread.h"
#include "Singleton.h"
#include "FileUtil.h"

namespace youth
{
class LogFile
{
    SINGLETON(LogFile)

    public:
        void setRollSize(off_t);
    void setBaseFileName(const std::string&);
    static void outputFunc(const char*, int);
    static void flushFunc(void);

    //这个方法是用来将日志写入Log文件
    void outputLogFile(const char*, int);
    void flushLogFile();

private:  
    std::string getFileName(time_t*);
    bool rollFile();

    off_t rollSize_ = 1000 * 1000 *1000;    //1G
    time_t startTime;
    time_t lastRoll;
    std::string basename;
    Mutex mutex;
    std::unique_ptr<FileUtil> file;
};
}

#endif /* LOGFILE_H */
