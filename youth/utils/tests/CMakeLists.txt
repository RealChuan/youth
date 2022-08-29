#Target
ADD_EXECUTABLE(SingletonTest SingletonTest.cpp)
target_link_libraries(SingletonTest youth_utils GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

ADD_EXECUTABLE(LogTest LogTest.cpp)
target_link_libraries(LogTest youth_utils)

ADD_EXECUTABLE(LogAsyncTest LogAsyncTest.cpp)
target_link_libraries(LogAsyncTest youth_utils)

ADD_EXECUTABLE(ThreadPoolTest ThreadPoolTest.cpp)
target_link_libraries(ThreadPoolTest youth_utils)

ADD_EXECUTABLE(LogMulti_unittest LogMulti_unittest.cc)
target_link_libraries(LogMulti_unittest youth_utils)

ADD_EXECUTABLE(LogSpeed_unitest LogSpeed_unitest.cc)
target_link_libraries(LogSpeed_unitest youth_utils benchmark::benchmark benchmark::benchmark_main glog::glog)

ADD_EXECUTABLE(Log_benchmark Log_benchmark.cc)
target_link_libraries(Log_benchmark youth_utils benchmark::benchmark benchmark::benchmark_main)
