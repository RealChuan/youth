#Target
ADD_EXECUTABLE(SingletonTest SingletonTest.cpp)
target_link_libraries(SingletonTest Youth)

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

ADD_EXECUTABLE(StringTest StringTest.cpp)
target_link_libraries(StringTest Youth)

ADD_EXECUTABLE(FileUtilTest FileUtilTest.cpp)
target_link_libraries(FileUtilTest Youth)

ADD_EXECUTABLE(ElapsedTimeTest ElapsedTimeTest.cpp)
target_link_libraries(ElapsedTimeTest Youth)

ADD_EXECUTABLE(CountDownLatchTest CountDownLatchTest.cpp)
target_link_libraries(CountDownLatchTest Youth)

ADD_EXECUTABLE(MutexTest MutexTest.cpp)
target_link_libraries(MutexTest Youth)

ADD_EXECUTABLE(ConditionTest ConditionTest.cpp)
target_link_libraries(ConditionTest Youth)
