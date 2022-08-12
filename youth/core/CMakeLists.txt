AUX_SOURCE_DIRECTORY(. DIR_CORE_SRCS)
ADD_LIBRARY (youth_core ${DIR_CORE_SRCS})
target_link_libraries(youth_core pthread)

ADD_SUBDIRECTORY(tests)
