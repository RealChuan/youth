AUX_SOURCE_DIRECTORY(. DIR_NET_SRCS)
ADD_LIBRARY (youth_net ${DIR_NET_SRCS})
target_link_libraries(youth_net youth_core youth_utils)

ADD_SUBDIRECTORY(tests)
