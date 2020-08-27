#Target
ADD_EXECUTABLE(EventLoopTest EventLoopTest.cpp)
target_link_libraries(EventLoopTest youth_net)

ADD_EXECUTABLE(TimerTest TimerTest.cpp)
target_link_libraries(TimerTest youth_net)

ADD_EXECUTABLE(TcpClientTest TcpClientTest.cpp)
target_link_libraries(TcpClientTest youth_net)
