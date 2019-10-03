/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TcpClient.h
 * Author: root
 *
 * Created on 2019年8月24日, 下午12:55
 */

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <netinet/in.h>

namespace youth
{
class TcpClient
{
public:
	TcpClient(const char* ip_, uint16_t port_);
	virtual ~TcpClient();
	int clientFd;
private:
	struct sockaddr_in serverAddr;
	uint16_t port;
	const char* ip;

};
}
#endif /* TCPCLIENT_H */
