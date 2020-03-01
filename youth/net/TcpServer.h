/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TcpServer.h
 * Author: root
 *
 * Created on 2019年8月24日, 下午4:21
 */

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <netinet/in.h>
#include <boost/function.hpp>

namespace youth
{
class TcpServer
{
	typedef std::function< void(int fd) > callBackFunc;
public:
	TcpServer(uint16_t port_,callBackFunc readCallBackFunc_);
	virtual ~TcpServer();
	void Loop();

private:
	struct sockaddr_in serverAddr;
	uint16_t port;
	int serverFd;

	callBackFunc readCallBackFunc;
};
}

#endif /* TCPSERVER_H */
