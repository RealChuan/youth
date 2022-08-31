#include <youth/core/Dir.h>

#include <gtest/gtest.h>

#include <iostream>

using namespace youth::core;

TEST(dirTest, Positive)
{
    auto dir = Dir::Current();
    std::cout << "path: " << dir.path() << std::endl;
    std::cout << "absolutePath: " << dir.absolutePath() << std::endl;
    std::cout << "dirname: " << dir.dirname() << std::endl;

    if (dir.mkdir("test1")) {
        EXPECT_TRUE(std::filesystem::exists("./test1"));
    } else {
        EXPECT_FALSE(std::filesystem::exists("./test1"));
    }
    if (dir.mkdir("test2")) {
        EXPECT_TRUE(std::filesystem::exists("./test2"));
    } else {
        EXPECT_FALSE(std::filesystem::exists("./test2"));
    }

    if (dir.rmdir("test1")) {
        EXPECT_FALSE(std::filesystem::exists("./test1"));
    } else {
        EXPECT_TRUE(std::filesystem::exists("./test1"));
    }
    if (dir.rmdir("test2")) {
        EXPECT_FALSE(std::filesystem::exists("./test2"));
    } else {
        EXPECT_TRUE(std::filesystem::exists("./test2"));
    }
}

TEST(staticDirTest, Positive)
{
    if (Dir::mkdirs("./test4/test/test")) {
        EXPECT_TRUE(std::filesystem::exists("./test4/test/test"));
    } else {
        EXPECT_FALSE(std::filesystem::exists("./test4/test/test"));
    }
    if (Dir::mkdirs("./test5/test/test")) {
        EXPECT_TRUE(std::filesystem::exists("./test5/test/test"));
    } else {
        EXPECT_FALSE(std::filesystem::exists("./test5/test/test"));
    }

    if (Dir::rmdirs("./test4")) {
        EXPECT_FALSE(std::filesystem::exists("./test4/test/test"));
    } else {
        EXPECT_TRUE(std::filesystem::exists("./test4/test/test"));
    }
    if (Dir::rmdirs("./test5")) {
        EXPECT_FALSE(std::filesystem::exists("./test5/test/test"));
    } else {
        EXPECT_TRUE(std::filesystem::exists("./test5/test/test"));
    }
}

TEST(staticFunctionDir, Positive)
{
    std::cout << "current path: " << Dir::Current().absolutePath() << std::endl;
    std::cout << "temp path: " << Dir::temp().absolutePath() << std::endl;
    std::cout << "isAbsolute: " << std::boolalpha << Dir::isAbsolute("./test") << std::endl;
    std::cout << "isRelative: " << std::boolalpha << Dir::isRelative("test") << std::endl;
    auto path = Dir::Current().path();
    std::cout << "fromNativeSeparators: " << Dir::fromNativeSeparators(path.string()) << std::endl;
    std::cout << "toNativeSeparators: " << Dir::toNativeSeparators(path) << std::endl;
}
