#Target
ADD_EXECUTABLE(SingletonTest SingletonTest.cpp)
target_link_libraries(SingletonTest youth_utils)

ADD_EXECUTABLE(LogTest LogTest.cpp)
target_link_libraries(LogTest youth_utils)

ADD_EXECUTABLE(LogAsyncTest LogAsyncTest.cpp)
target_link_libraries(LogAsyncTest youth_utils)

ADD_EXECUTABLE(ThreadPoolTest ThreadPoolTest.cpp)
target_link_libraries(ThreadPoolTest youth_utils)

ADD_EXECUTABLE(LogSpeed_unitest LogSpeed_unitest.cc)
target_link_libraries(LogSpeed_unitest youth_utils)
