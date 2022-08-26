#include <youth/core/StringFunction.hpp>

#include <gtest/gtest.h>

using namespace youth::core;

TEST(splitTest, Positive)
{
    std::string str = "a,b,c,d";
    auto result = string::split(str, ",");

    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "c");
    EXPECT_EQ(result[3], "d");
}

TEST(removeTest, Positive)
{
    std::string str = "a,b,c,d";
    string::remove(str, ",");
    EXPECT_EQ(str, "abcd");
    string::remove(str, ".");
    EXPECT_EQ(str, "abcd");
    string::remove(str, "a");
    EXPECT_EQ(str, "bcd");
    string::remove(str, "z");
    EXPECT_EQ(str, "bcd");
    string::remove(str, "b");
    EXPECT_EQ(str, "cd");
    string::remove(str, "c");
    EXPECT_EQ(str, "d");
    string::remove(str, "d");
    EXPECT_TRUE(str.empty());
}

TEST(replaceTest, Positive)
{
    std::string str = "a,b,c,d";
    string::replace(str, ",", ":");
    EXPECT_EQ(str, "a:b:c:d");
    string::replace(str, "a", "A");
    EXPECT_EQ(str, "A:b:c:d");
    string::replace(str, "b", "B");
    EXPECT_EQ(str, "A:B:c:d");
    string::replace(str, "c", "C");
    EXPECT_EQ(str, "A:B:C:d");
    string::replace(str, "d", "D");
    EXPECT_EQ(str, "A:B:C:D");
}

TEST(trimmedTest, Positive)
{
    std::string str = " a,b,c,d ";
    str = string::trimmed(str);
    EXPECT_EQ(str, "a,b,c,d");
    str = string::trimmed(str);
    EXPECT_EQ(str, "a,b,c,d");
    str = "\ta,b,c,d\t";
    str = string::trimmed(str);
    EXPECT_EQ(str, "a,b,c,d");
    str = "\na,b,c,d\n";
    str = string::trimmed(str);
    EXPECT_EQ(str, "a,b,c,d");
    str = "\ra,b,c,d\r";
    str = string::trimmed(str);
    EXPECT_EQ(str, "a,b,c,d");
}

TEST(toLowerTest, Positive)
{
    std::string str = "a,b,c,d";
    str = string::toLower(str);
    EXPECT_EQ(str, "a,b,c,d");
    str = "A,b,c,d";
    str = string::toLower(str);
    EXPECT_EQ(str, "a,b,c,d");
    str = "A,B,c,d";
    str = string::toLower(str);
    EXPECT_EQ(str, "a,b,c,d");
    str = "A,B,C,d";
    str = string::toLower(str);
    EXPECT_EQ(str, "a,b,c,d");
    str = "A,B,C,D";
    str = string::toLower(str);
    EXPECT_EQ(str, "a,b,c,d");
}

TEST(toUpperTest, Positive)
{
    std::string str = "a,b,c,d";
    str = string::toUpper(str);
    EXPECT_EQ(str, "A,B,C,D");
}

TEST(startsWithTest, Positive)
{
    std::string text = "abcdef";
    std::string str = "abc";
    EXPECT_TRUE(string::startsWith(text, str));
    str = "def";
    EXPECT_FALSE(string::startsWith(text, str));
}

TEST(endsWithTest, Positive)
{
    std::string text = "abcdef";
    std::string str = "abc";
    EXPECT_FALSE(string::endsWith(text, str));
    str = "def";
    EXPECT_TRUE(string::endsWith(text, str));
}

TEST(containsTest, Positive)
{
    std::string text = "abcdef";
    std::string str = "abc";
    EXPECT_TRUE(string::contains(text, str));
    str = "ac";
    EXPECT_FALSE(string::contains(text, str));
}

TEST(isLowerTest, Positive)
{
    std::string str = "a";
    EXPECT_TRUE(string::isLower(str));
    str = "A";
    EXPECT_FALSE(string::isLower(str));
}

TEST(isUppeTest, Positive)
{
    std::string str = "a";
    EXPECT_FALSE(string::isUpper(str));
    str = "A";
    EXPECT_TRUE(string::isUpper(str));
}
