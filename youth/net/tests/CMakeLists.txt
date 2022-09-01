#Target
ADD_EXECUTABLE(EventLoopTest EventLoopTest.cpp)
target_link_libraries(EventLoopTest youth_net)
#add_test(NAME EventLoopTest COMMAND EventLoopTest)

ADD_EXECUTABLE(TimerTest TimerTest.cpp)
target_link_libraries(TimerTest youth_net)

ADD_EXECUTABLE(TcpClientTest TcpClientTest.cpp)
target_link_libraries(TcpClientTest youth_net)

ADD_EXECUTABLE(TcpServerTest TcpServerTest.cpp)
target_link_libraries(TcpServerTest youth_net)

ADD_EXECUTABLE(Resolve_unittest Resolve_unittest.cc)
target_link_libraries(Resolve_unittest youth_net)
add_test(NAME Resolve_unittest COMMAND Resolve_unittest)
