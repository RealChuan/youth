/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProcessMsg.h
 * Author: root
 *
 * Created on 2019年8月4日, 上午9:07
 */

#ifndef PROCESSMSG_H
#define PROCESSMSG_H

#include <string>

namespace youth
{
namespace processmsg
{

pid_t getTid();
pid_t getPid();
std::string hostname();
std::string fileBaseName(const char*);

}
}

#endif /* PROCESSMSG_H */
