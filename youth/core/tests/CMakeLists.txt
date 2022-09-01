ADD_EXECUTABLE(CountDownLatch_unittest CountDownLatch_unittest.cc)
target_link_libraries(CountDownLatch_unittest youth_core)
add_test(NAME CountDownLatch_unittest COMMAND CountDownLatch_unittest)

ADD_EXECUTABLE(DateTime_unittest DateTime_unittest.cc)
target_link_libraries(DateTime_unittest youth_core GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
gtest_discover_tests(DateTime_unittest)

ADD_EXECUTABLE(ElapsedTimer_unittest ElapsedTimer_unittest.cc)
target_link_libraries(ElapsedTimer_unittest youth_core GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
gtest_discover_tests(ElapsedTimer_unittest)

ADD_EXECUTABLE(File_unittest File_unittest.cc)
target_link_libraries(File_unittest youth_core)
gtest_discover_tests(File_unittest)

ADD_EXECUTABLE(Dir_unittest Dir_unittest.cc)
target_link_libraries(Dir_unittest youth_core GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
gtest_discover_tests(Dir_unittest)

ADD_EXECUTABLE(StringFunction_unittest StringFunction_unittest.cc)
target_link_libraries(StringFunction_unittest youth_core GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
gtest_discover_tests(StringFunction_unittest)

ADD_EXECUTABLE(FileInfo_unittest FileInfo_unittest.cc)
target_link_libraries(FileInfo_unittest youth_core)
add_test(NAME FileInfo_unittest COMMAND FileInfo_unittest)

ADD_EXECUTABLE(Thread_unittest Thread_unittest.cc)
target_link_libraries(Thread_unittest youth_core GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
gtest_discover_tests(Thread_unittest)

ADD_EXECUTABLE(Process_unittest Process_unittest.cc)
target_link_libraries(Process_unittest youth_core)
add_test(NAME Process_unittest COMMAND Process_unittest)
