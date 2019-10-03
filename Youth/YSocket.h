/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   YSocket.h
 * Author: root
 *
 * Created on 2019年8月20日, 下午8:56
 */

#ifndef YSOCKET_H
#define YSOCKET_H

#include <string>
#include <netinet/in.h>

#include <boost/implicit_cast.hpp>

namespace youth
{
    namespace YSocket
    {
        struct sockaddr_in addrServer(uint16_t port);

        int socket();

        void listenAndBindServer(int serverfd, struct sockaddr_in serverAddr);

        int acceptServer(int serverfd, struct sockaddr_in clientAddr);

        struct sockaddr_in addrClient(const char* ip, uint16_t port);

        void connectServer(int sockfd, struct sockaddr_in serverAddr_);

        void closeSock(int sockfd);

        std::string getIpAndPort(struct sockaddr_in addr);

        std::string getLocalIpAndPort(int sockfd);

    };
}

#endif /* YSOCKET_H */

