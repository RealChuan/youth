#include <youth/core/Dir.h>

#include <iostream>

using namespace youth::core;

int main(int argc, char *argv[])
{
    auto dir = Dir::Current();
    std::cout << "path: " << dir.path() << std::endl;
    std::cout << "absolutePath: " << dir.absolutePath() << std::endl;
    std::cout << "dirname: " << dir.dirname() << std::endl;

    std::cout << "dir mkdirs" << std::endl;
    std::cout << std::boolalpha << dir.mkdir("test1") << std::endl;
    std::cout << std::boolalpha << dir.mkdir("test2") << std::endl;
    std::cout << std::boolalpha << dir.mkdir("test3") << std::endl;

    std::cout << "Dir mkdirs" << std::endl;
    std::cout << std::boolalpha << Dir::mkdirs("./test4/test/test") << std::endl;
    std::cout << std::boolalpha
              << Dir::mkdirs("./test5/test/test", std::filesystem::perms::owner_all) << std::endl;
    std::cout << std::boolalpha
              << Dir::mkdirs("./test6/test/test", std::filesystem::perms::owner_read) << std::endl;

    std::cout << "dir rmdir" << std::endl;
    std::cout << std::boolalpha << dir.rmdir("test1") << std::endl;
    std::cout << std::boolalpha << dir.rmdir("test2") << std::endl;
    std::cout << std::boolalpha << dir.rmdir("test3") << std::endl;

    std::cout << "Dir rmdir" << std::endl;
    std::cout << std::boolalpha << Dir::rmdirs("./test4") << std::endl;
    std::cout << std::boolalpha << Dir::rmdirs("./test5") << std::endl;
    std::cout << std::boolalpha << Dir::rmdirs("./test6") << std::endl;

    std::cout << "current path: " << Dir::Current().absolutePath() << std::endl;
    std::cout << "temp path: " << Dir::temp().absolutePath() << std::endl;
    std::cout << "isAbsolute: " << std::boolalpha << Dir::isAbsolute("./test") << std::endl;
    std::cout << "isRelative: " << std::boolalpha << Dir::isRelative("test") << std::endl;
    auto path = dir.path();
    std::cout << "fromNativeSeparators: " << Dir::fromNativeSeparators(path) << std::endl;
    std::cout << "toNativeSeparators: " << Dir::toNativeSeparators(path) << std::endl;

    return 0;
}