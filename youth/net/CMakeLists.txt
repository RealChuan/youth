set(NET_SRCS
        Acceptor.cpp
        Acceptor.h
        Buffer.h
        Buffer.cpp
        Callbacks.h
        Channel.cpp
        Channel.h
        Connector.cpp
        Connector.h
        Epoll.cpp
        Epoll.h
        EventLoop.h
        EventLoop.cpp
        EventLoopThread.cpp
        EventLoopThread.h
        EventLoopThreadPool.cpp
        EventLoopThreadPool.h
        HostAddress.cc
        HostAddress.hpp
        Socket.cpp
        Socket.h
        SocketFunc.cpp
        SocketFunc.h
        TcpClient.cpp
        TcpClient.h
        TcpConnection.cpp
        TcpConnection.h
        TcpServer.cpp
        TcpServer.h
        Timer.cpp
        Timer.h
        TimerId.h
        TimerQueue.h
        TimerQueue.cpp)

ADD_LIBRARY (youth_net ${NET_SRCS})
target_link_libraries(youth_net youth_core youth_utils)

ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(http)

if(PROTOBUF_FOUND)
    ADD_SUBDIRECTORY(rpc)
endif()
