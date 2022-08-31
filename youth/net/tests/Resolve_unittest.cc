#include <youth/net/HostAddress.hpp>
#include <youth/utils/Logging.h>

using namespace youth::net;
using namespace youth::utils;

void resolve(std::string_view host)
{
    LOG_INFO << host.data();
    auto addresses = HostAddress::resolve(host);
    for (auto &address : addresses) {
        LOG_INFO << address.ipAndPort();
    }
    LOG_INFO << "--------------------------------------------------------------------";
}

int main(int argc, char **argv)
{
    resolve("baidu.com");
    resolve("tencent.com");
    resolve("alibabagroup.com");
    resolve("github.com");
    resolve("realchuan.github.io");

    return 0;
}
