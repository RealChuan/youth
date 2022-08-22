set(CORE_SRCS
        CountDownLatch.cpp
        CountDownLatch.h
        CurrentThread.cpp
        CurrentThread.h
        DateTime.cc
        DateTime.hpp
        Dir.cpp
        Dir.h
        ElapsedTimer.cpp
        ElapsedTimer.h
        File.cc
        File.hpp
        FileInfo.cc
        FileInfo.hpp
        Object.h
        Process.cc
        Process.hpp
        StringFunction.cc
        StringFunction.hpp
        Thread.cc
        Thread.hpp
        WeakCallback.h)

ADD_LIBRARY (youth_core ${CORE_SRCS})
target_link_libraries(youth_core pthread)

ADD_SUBDIRECTORY(tests)
