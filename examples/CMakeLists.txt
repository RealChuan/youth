#Target
ADD_EXECUTABLE(LogTest LogTest.cpp)
target_link_libraries(LogTest Youth)

ADD_EXECUTABLE(LogAsyncTest LogAsyncTest.cpp)
target_link_libraries(LogAsyncTest Youth)

ADD_EXECUTABLE(ThreadPoolTest ThreadPoolTest.cpp)
target_link_libraries(ThreadPoolTest Youth)

ADD_EXECUTABLE(TcpClientTest TcpClientTest.cpp)
target_link_libraries(TcpClientTest Youth)

ADD_EXECUTABLE(TcpServerTest TcpServerTest.cpp)
target_link_libraries(TcpServerTest Youth)
