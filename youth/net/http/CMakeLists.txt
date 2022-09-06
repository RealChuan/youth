set(HTTP_SRCS
        HttpContext.cpp
        HttpContext.h
        HttpRequest.cpp
        HttpRequest.h
        HttpResponse.cpp
        HttpResponse.h
        HttpServer.cpp
        HttpServer.h
        HttpMethod.cc
        HttpMethod.hpp)

ADD_LIBRARY (youth_net_http ${HTTP_SRCS})
target_link_libraries(youth_net_http youth_core youth_utils youth_net)

ADD_SUBDIRECTORY(tests)
