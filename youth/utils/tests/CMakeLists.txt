#Target
ADD_EXECUTABLE(SingletonTest SingletonTest.cpp)
target_link_libraries(SingletonTest youth_utils)

ADD_EXECUTABLE(LogTest LogTest.cpp)
target_link_libraries(LogTest youth_utils)

ADD_EXECUTABLE(LogAsyncTest LogAsyncTest.cpp)
target_link_libraries(LogAsyncTest youth_utils)

ADD_EXECUTABLE(ThreadPoolTest ThreadPoolTest.cpp)
target_link_libraries(ThreadPoolTest youth_utils)

ADD_EXECUTABLE(ElapsedTimeTest ElapsedTimeTest.cpp)
target_link_libraries(ElapsedTimeTest youth_utils)

ADD_EXECUTABLE(CountDownLatchTest CountDownLatchTest.cpp)
target_link_libraries(CountDownLatchTest youth_utils)

ADD_EXECUTABLE(MutexTest MutexTest.cpp)
target_link_libraries(MutexTest youth_core)
target_link_libraries(MutexTest youth_utils)

ADD_EXECUTABLE(ConditionTest ConditionTest.cpp)
target_link_libraries(ConditionTest youth_utils)
