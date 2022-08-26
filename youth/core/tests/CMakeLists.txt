ADD_EXECUTABLE(CountDownLatch_unittext CountDownLatch_unittext.cc)
target_link_libraries(CountDownLatch_unittext youth_core)

ADD_EXECUTABLE(DateTime_unittest DateTime_unittest.cc)
target_link_libraries(DateTime_unittest youth_core GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

ADD_EXECUTABLE(ElapsedTimer_unittest ElapsedTimer_unittest.cc)
target_link_libraries(ElapsedTimer_unittest youth_core GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

ADD_EXECUTABLE(File_unittest File_unittest.cc)
target_link_libraries(File_unittest youth_core)

ADD_EXECUTABLE(Dir_unittest Dir_unittest.cc)
target_link_libraries(Dir_unittest youth_core GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

ADD_EXECUTABLE(StringFunction_unittest StringFunction_unittest.cc)
target_link_libraries(StringFunction_unittest youth_core GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

ADD_EXECUTABLE(FileInfo_unittest FileInfo_unittest.cc)
target_link_libraries(FileInfo_unittest youth_core)

ADD_EXECUTABLE(Thread_unittest Thread_unittest.cc)
target_link_libraries(Thread_unittest youth_core GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

ADD_EXECUTABLE(Process_unittest Process_unittest.cc)
target_link_libraries(Process_unittest youth_core)
