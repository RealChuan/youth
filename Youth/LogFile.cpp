/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LogFile.cpp
 * Author: root
 *
 * Created on 2019年8月4日, 上午7:24
 */

#include "LogFile.h"
#include "ProcessMsg.h"
#include "Time.h"
#include <sys/stat.h>
#include <sys/types.h>

using namespace youth;

LogFile::LogFile()
{
	openFile();
}

LogFile::~LogFile()
{
	if (fpLog)
	{
		fclose(fpLog);
		fpLog = nullptr;
	}
}

void LogFile::setFileName(std::string basename_)
{
    basename = basename_;
}

void LogFile::outputFunc(const char *msg, int len)
{
    LogFile *pLogFile = &instance();
    pLogFile->outputLogFile(msg, len);
}

void LogFile::flushFunc()
{
    LogFile *pLogFile = &instance();
    pLogFile->flushLogFile();
}

void LogFile::outputLogFile(const char *msg, int len)
{
    MutexLock lock(mutex);
	if (fpLog != nullptr)
        ::fwrite(msg, 1, static_cast<size_t>(len), fpLog);
	else
		printf("No File To Write!\n");
}

void LogFile::flushLogFile()
{
    MutexLock lock(mutex);
	::fflush(fpLog);
}

std::string LogFile::getFileName()
{

	Time _time;

	std::string _fileName;

	if (access("Log/", F_OK) == -1)
		mkdir("Log/", 0777);

	char fileNameBuf[64];
	memset(fileNameBuf, 0, sizeof fileNameBuf);
	snprintf(fileNameBuf, sizeof (fileNameBuf), "%s%s%s.%s.%d%s",
             "Log/", basename.c_str(), _time.getLogDay().c_str(), processmsg::hostname().c_str(), processmsg::getPid(), ".log");

	_fileName += fileNameBuf;

	return _fileName;
}

void LogFile::openFile()
{
	newFileName = LogFile::getFileName();
	if (newFileName != fileName)
	{
		if (fpLog)
		{
			fclose(fpLog);
			fpLog = nullptr;
		}
		fileName = newFileName;

		//printf("FIleName=%s\n", fileName.c_str());

		fpLog = fopen(fileName.c_str(), "a");
	}
}
