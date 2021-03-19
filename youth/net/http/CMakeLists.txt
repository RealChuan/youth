AUX_SOURCE_DIRECTORY(. DIR_HTTP_SRCS)
ADD_LIBRARY (youth_net_http ${DIR_HTTP_SRCS})
target_link_libraries(youth_net_http youth_core youth_utils youth_net)

ADD_SUBDIRECTORY(tests)
