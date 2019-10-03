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
#include "Socket.h"
#include "LogOut.h"
#include "Epoll.h"

using namespace youth;

TcpServer::TcpServer(uint16_t port_,callBackFunc readCallBackFunc_)
	: port(port_)
	,readCallBackFunc(std::move(readCallBackFunc_))
{
	serverAddr = Socket::addrServer(port);
	serverFd = Socket::socket();
	Socket::listenAndBindServer(serverFd, serverAddr);
}

TcpServer::~TcpServer()
{
	Socket::closeSock(serverFd);
}

void TcpServer::Loop()
{
	Epoll myEpoll(serverFd,readCallBackFunc);
	myEpoll.Loop_1();
}

