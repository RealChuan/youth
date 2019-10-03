/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TcpClient.cpp
 * Author: root
 * 
 * Created on 2019年8月24日, 下午12:55
 */

#include "TcpClient.h"
#include "YSocket.h"

using namespace youth;

TcpClient::TcpClient(const char* ip_, uint16_t port_)
:port(port_)
,ip(ip_)
{
    serverAddr = YSocket::addrClient(ip, port);
    clientFd = YSocket::socket();
    YSocket::connectServer(clientFd, serverAddr);
}

TcpClient::~TcpClient()
{
    YSocket::closeSock(clientFd);
}

