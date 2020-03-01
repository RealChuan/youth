#Target
ADD_EXECUTABLE(StringTest StringTest.cpp)
target_link_libraries(StringTest youth_core)

ADD_EXECUTABLE(FileUtilTest FileUtilTest.cpp)
target_link_libraries(FileUtilTest youth_core)
