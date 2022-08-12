AUX_SOURCE_DIRECTORY(. DIR_RPC_SRCS)
ADD_LIBRARY (youth_net_rpc ${DIR_RPC_SRCS})
target_link_libraries(youth_net_rpc PRIVATE youth_core youth_utils youth_net ZLIB::ZLIB protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
