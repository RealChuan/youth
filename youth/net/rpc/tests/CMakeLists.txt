add_custom_command(OUTPUT resolveer.pb.cc resolveer.pb.h
  COMMAND /usr/local/share/vcpkg/installed/x64-linux/tools/protobuf/protoc
  ARGS --cpp_out . ${CMAKE_CURRENT_SOURCE_DIR}/resolveer.proto -I${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS resolveer.proto
  VERBATIM )

include_directories(${PROJECT_BINARY_DIR})

add_executable(RpcServer_unittest server.cc resolveer.pb.cc resolveer.pb.h)
target_link_libraries(RpcServer_unittest youth_net_rpc protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

add_executable(RpcClinet_unittest client.cc resolveer.pb.cc resolveer.pb.h)
target_link_libraries(RpcClinet_unittest youth_net_rpc protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)