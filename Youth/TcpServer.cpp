/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TcpServer.cpp
 * Author: root
 * 
 * Created on 2019年8月24日, 下午4:21
 */

#include "TcpServer.h"
#include "YSocket.h"

using namespace youth;

TcpServer::TcpServer(uint16_t port_)
: port(port_)
{
    serverAddr = YSocket::addrServer(port);
    serverFd = YSocket::socket();
    YSocket::listenAndBindServer(serverFd, serverAddr);
    acceptFd = YSocket::acceptServer(serverFd,serverAddr);
}

TcpServer::~TcpServer()
{
    YSocket::closeSock(acceptFd);
    YSocket::closeSock(serverFd);
}

