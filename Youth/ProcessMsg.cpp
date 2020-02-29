/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProcessMsg.cpp
 * Author: root
 *
 * Created on 2019年8月4日, 上午9:07
 */

#include "ProcessMsg.h"
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h> 

using namespace youth;

pid_t ProcessMsg::getTid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

pid_t ProcessMsg::getPid()
{
	return ::getpid();
}

std::string ProcessMsg::hostname()
{
	// HOST_NAME_MAX 64
	// _POSIX_HOST_NAME_MAX 255
	char buf[256];
	if (::gethostname(buf, sizeof buf) == 0){
		buf[sizeof (buf) - 1] = '\0';
		return buf;
	}
	else
		return "unknownhost";
}

std::string ProcessMsg::fileBaseName(const char* basename)
{
	const char *pTmp = strrchr(basename, '/');
	std::string _basename;
	//int Len = 0;

	if (pTmp != nullptr){
		_basename += pTmp + 1;
		//Len = static_cast<int> (strlen(_basename));
	}
	return _basename;
}
