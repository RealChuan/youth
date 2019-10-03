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

namespace youth
{

class TcpServer
{
public:
	TcpServer(uint16_t port_);
	virtual ~TcpServer();

	int acceptFd;
private:
	struct sockaddr_in serverAddr, clientAddr;
	uint16_t port;
	int serverFd;

};
}

#endif /* TCPSERVER_H */
