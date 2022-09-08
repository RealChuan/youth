set(HTTP_SRCS
        GetMethod.hpp
        GetMethod.cc
        HeadMethod.hpp
        HeadMethod.cc
        PostMethod.hpp
        PostMethod.cc
        main.cc)

add_executable(StandardHttpServer ${HTTP_SRCS})
target_link_libraries(StandardHttpServer youth_net_http youth_utils youth_core)
