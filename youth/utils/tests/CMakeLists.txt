#Target
ADD_EXECUTABLE(SingletonTest SingletonTest.cpp)
target_link_libraries(SingletonTest youth_utils GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
gtest_discover_tests(SingletonTest)

ADD_EXECUTABLE(LogTest LogTest.cpp)
target_link_libraries(LogTest youth_utils)
add_test(NAME LogTest COMMAND LogTest)

ADD_EXECUTABLE(LogAsyncTest LogAsyncTest.cpp)
target_link_libraries(LogAsyncTest youth_utils)
add_test(NAME LogAsyncTest COMMAND LogAsyncTest)

ADD_EXECUTABLE(ThreadPoolTest ThreadPoolTest.cpp)
target_link_libraries(ThreadPoolTest youth_utils)
add_test(NAME ThreadPoolTest COMMAND ThreadPoolTest)

ADD_EXECUTABLE(LogMulti_unittest LogMulti_unittest.cc)
target_link_libraries(LogMulti_unittest youth_utils)
add_test(NAME LogMulti_unittest COMMAND LogMulti_unittest)

ADD_EXECUTABLE(LogSpeed_unitest LogSpeed_unitest.cc)
target_link_libraries(LogSpeed_unitest youth_utils benchmark::benchmark benchmark::benchmark_main glog::glog)
add_test(NAME LogSpeed_unitest COMMAND LogSpeed_unitest)

ADD_EXECUTABLE(Log_benchmark Log_benchmark.cc)
target_link_libraries(Log_benchmark youth_utils benchmark::benchmark benchmark::benchmark_main)
add_test(NAME Log_benchmark COMMAND Log_benchmark)

ADD_EXECUTABLE(BreakPad_unittest BreakPad_unittest.cc)
target_link_libraries(BreakPad_unittest youth_utils)
