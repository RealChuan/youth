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
#include "YThread.h"

namespace youth
{
class LogFile
{
public:
	LogFile();
	virtual ~LogFile();

	static void setFileName(std::string);
	static void outputFunc(const char *pMsg, int Len);
	static void flushFunc(void);

	//这个方法是用来将日志写入Log文件
	void outputLogFile(const char *pMsg, int Len);
	void flushLogFile();

	//单例模式
	static LogFile *instance();

private:
	std::string getFileName();
	void openFile();

	std::string fileName;
	std::string newFileName;
	FILE* fpLog;
	YMutex myMutex;
};
}

#endif /* LOGFILE_H */
