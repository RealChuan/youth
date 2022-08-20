#include <youth/core/File.hpp>
#include <youth/core/Process.hpp>

#include <iostream>

using namespace youth::core;

int main(int argc, char **argv)
{
    std::cout << "getTid(): " << Process::getTid() << std::endl;
    std::cout << "getPid(): " << Process::getPid() << std::endl;
    std::cout << "hostname(): " << Process::hostname() << std::endl;
    std::cout << "fileBaseName(argv[0]): " << Process::fileBaseName(argv[0]) << std::endl;
    std::cout << "excute(\"ls -l\"): " << Process::excute("ls -l") << std::endl;
    std::cout << "getPidFromName(\"Process_unittest\"): "
              << Process::getPidFromName("Process_unittest") << std::endl;
    std::cout << "getCwd(): " << Process::getCwd() << std::endl;
    auto env = Process::systemEnvironment();
    for (auto &e : env) {
        std::cout << e << std::endl;
    }
    //std::cout << "setEnvironment(\"TEST\", \"TEST\"): " << Process::setEnvironment("TEST", "TEST") << std::endl;
    std::cout << "nullDevice(): " << Process::nullDevice().fileName() << std::endl;

    return 0;
}
