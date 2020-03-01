AUX_SOURCE_DIRECTORY(. DIR_UTILS_SRCS)
ADD_LIBRARY (youth_utils ${DIR_UTILS_SRCS})
target_link_libraries(youth_utils youth_core)

ADD_SUBDIRECTORY(tests)
