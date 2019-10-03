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
#include "YTime.h"
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

std::string static basename_;

void LogFile::setFileName(std::string basename)
{
	basename_ = basename;
}

void LogFile::outputFunc(const char *pMsg, int Len)
{
	LogFile *pLogFile = instance();
	pLogFile->outputLogFile(pMsg, Len);
}

void LogFile::flushFunc()
{
	LogFile *pLogFile = instance();
	pLogFile->flushLogFile();
}

void LogFile::outputLogFile(const char *pMsg, int Len)
{
	YMutexLock lock(myMutex);
	if (fpLog != nullptr)
		::fwrite(pMsg, 1, static_cast<size_t>(Len), fpLog);
	else
		printf("No File To Write!\n");
}

void LogFile::flushLogFile()
{
	YMutexLock lock(myMutex);
	::fflush(fpLog);
}

LogFile *LogFile::instance()
{
	static LogFile singleLogFile;

	return &singleLogFile;
}

std::string LogFile::getFileName()
{

	YTime _time;

	std::string _fileName;

	if (access("Log/", F_OK) == -1)
		mkdir("Log/", 0777);

	char fileNameBuf[64];
	memset(fileNameBuf, 0, sizeof fileNameBuf);
	snprintf(fileNameBuf, sizeof (fileNameBuf), "%s%s%s.%s.%d%s",
			 "Log/", basename_.c_str(), _time.getLogDay().c_str(), processmsg::hostname().c_str(), processmsg::getPid(), ".log");

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
