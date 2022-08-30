#include <youth/net/HostAddress.hpp>
#include <youth/utils/Logging.h>

using namespace youth::net;
using namespace youth::utils;

int main(int argc, char **argv)
{
    auto addresses = HostAddress::resolve("baidu.com");
    for (auto &address : addresses) {
        LOG_INFO << address.ipAndPort();
    }

    return 0;
}
