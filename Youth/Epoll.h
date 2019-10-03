#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include <boost/function.hpp>

#define MAX_EVENTS 256

namespace youth
{
class Epoll
{
	typedef std::function< void(int fd) > callBackFunc;
public:
	Epoll(int serverFd,callBackFunc readCallBackFunc_);
	~Epoll();

	//单线程
	void Loop_1();

private:
	int serverFd;
	int acceptFd;
	// epoll 初始化
	int epollfd;
	int timeout;

	struct epoll_event event;
	//事件数组
	struct epoll_event eventList[MAX_EVENTS];

	callBackFunc readCallBackFunc;
};
}
#endif // EPOLL_H
