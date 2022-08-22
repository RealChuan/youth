set(UTILS_SRCS
        LogAsync.cpp
        LogAsync.h
        LogBuffer.h
        LogFile.cpp
        LogFile.h
        Logging.cpp
        Logging.h
        LogOut.cpp
        LogOut.h
        LogStream.cpp
        LogStream.h
        Singleton.h
        TaskQueue.h
        ThreadPool.cpp
        ThreadPool.h)

ADD_LIBRARY (youth_utils ${UTILS_SRCS})
target_link_libraries(youth_utils youth_core)

ADD_SUBDIRECTORY(tests)
