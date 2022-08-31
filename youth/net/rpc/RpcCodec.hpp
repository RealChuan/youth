#pragma once

#include "ProtobufCodecLite.hpp"

namespace youth {

namespace rpc {

class RpcMessage;
using RpcMessagePtr = std::shared_ptr<RpcMessage>;

extern const char rpctag[5]; // = "RPC0";
using RpcCodec = ProtobufCodecLiteT<RpcMessage, rpctag>;

} // namespace rpc

} // namespace youth
