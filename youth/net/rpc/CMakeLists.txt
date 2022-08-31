add_custom_command(OUTPUT rpc.pb.cc rpc.pb.h
  COMMAND /usr/local/share/vcpkg/installed/x64-linux/tools/protobuf/protoc
  ARGS --cpp_out . ${CMAKE_CURRENT_SOURCE_DIR}/rpc.proto -I${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS rpc.proto
  VERBATIM )

include_directories(${PROJECT_BINARY_DIR})

set(RPC_SRCS
        BufferStream.hpp
        ProtobufCodecLite.cc
        ProtobufCodecLite.hpp
        RpcChannel.cc
        RpcChannel.hpp
        RpcCodec.cc
        RpcCodec.hpp
        RpcServer.cc
        RpcServer.hpp)

ADD_LIBRARY (youth_net_rpc ${RPC_SRCS} rpc.pb.cc rpc.pb.h)
target_link_libraries(youth_net_rpc PRIVATE youth_core youth_utils youth_net ZLIB::ZLIB protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
