#include <youth/net/TcpAddressInfo.h>
#include <youth/utils/Logging.h>

using namespace youth::net;
using namespace youth::utils;

int main(int argc, char **argv)
{
    auto addresses = TcpAddressInfo::resolve("baidu.com");
    for (auto &address : addresses) {
        LOG_INFO << address.ipAndPort();
    }

    return 0;
}
