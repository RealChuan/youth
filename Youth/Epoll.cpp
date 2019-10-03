#include "Epoll.h"
#include "LogOut.h"
#include "Socket.h"

using namespace youth;

Epoll::Epoll(int serverFd_,callBackFunc readCallBackFunc_)
	:serverFd(serverFd_)
	,timeout(3000)
	,readCallBackFunc(std::move(readCallBackFunc_))
{
	epollfd = epoll_create(MAX_EVENTS);//
	event.events = EPOLLIN|EPOLLET;
	event.data.fd = serverFd;
}

Epoll::~Epoll()
{
	Socket::closeSock(epollfd);
}

void Epoll::Loop_1()
{
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, serverFd, &event) < 0)
	{
		LOG_FATAL<<"Epoll Add Fail : Fd ="<<serverFd<<"error code="<<errno;
		return;
	}
	//epoll
	while(1)
	{
		//epoll_wait
		int ret = epoll_wait(epollfd, eventList, MAX_EVENTS, timeout);
		if(ret < 0)
		{
			LOG_FATAL<<"epoll error\n"<<"error code="<<errno;
			break;
		}
		else if(ret == 0)
		{
			LOG_INFO<<"timeout ...";
			continue;
		}

		//直接获取了事件数量,给出了活动的流,这里是和poll区别的关键
		int i = 0;
		for(i=0; i<ret; i++)
		{
			//错误退出
			if ((eventList[i].events & EPOLLERR) ||
					(eventList[i].events & EPOLLHUP) ||
					!(eventList[i].events & EPOLLIN))
			{
				LOG_INFO<< "epoll error\n"<<"error code="<<errno;
				Socket::closeSock (eventList[i].data.fd);
				//break;
			}

			if (eventList[i].data.fd == serverFd)
			{
				/********************************************************/
				/*可以在此增加开辟多线程，实现每个fd由一个线程进行监控并recv数据，*/
				/*然后在开辟一个线程池，把decode，endecode，send函数放入线程池,*/
				/*以免在进行数据处理的时候，无法从网络recv数据,				*/
				/*即reactor+多线程IO/recv+decode/encode/send线程池模型     */
				/********************************************************/
				acceptFd = Socket::acceptServer(serverFd);
				//将新建立的连接添加到EPOLL的监听中
				struct epoll_event event_;
				event_.data.fd = acceptFd;
				event_.events =  EPOLLIN|EPOLLET;
				epoll_ctl(epollfd, EPOLL_CTL_ADD, acceptFd, &event_);
			}
			else
			{
				readCallBackFunc(eventList[i].data.fd);
			}
		}
	}
}
