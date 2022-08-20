#include <youth/core/StringFunction.hpp>

#include <assert.h>
#include <iostream>

using namespace youth::core;

void test_string_split()
{
    std::cout << "test_string_split" << std::endl;
    std::string str = "a,b,c,d";
    std::vector<std::string> result = string::split(str, ",");
    assert(result.size() == 4);
    assert(result[0] == "a");
    assert(result[1] == "b");
    assert(result[2] == "c");
    assert(result[3] == "d");
    std::cout << "test_string_split OK" << std::endl;
}

void test_string_remove()
{
    std::cout << "test_remove" << std::endl;
    std::string str = "a,b,c,d";
    assert(string::remove(str, ",") == true);
    assert(str == "a,b,c,d");
    assert(string::remove(str, "a") == true);
    assert(str == "b,c,d");
    assert(string::remove(str, "b") == true);
    assert(str == "c,d");
    assert(string::remove(str, "c") == true);
    assert(str == "d");
    assert(string::remove(str, "d") == true);
    assert(str == "");
    std::cout << "test_remove OK" << std::endl;
}

void test_string_replace()
{
    std::cout << "test_replace" << std::endl;
    std::string str = "a,b,c,d";
    assert(string::replace(str, ",", ":") == true);
    assert(str == "a,b,c,d");
    assert(string::replace(str, "a", "A") == true);
    assert(str == "A,b,c,d");
    assert(string::replace(str, "b", "B") == true);
    assert(str == "A,B,c,d");
    assert(string::replace(str, "c", "C") == true);
    assert(str == "A,B,C,d");
    assert(string::replace(str, "d", "D") == true);
    assert(str == "A,B,C,D");
    std::cout << "test_replace OK" << std::endl;
}

void test_string_trimmed()
{
    std::cout << "test_trimmed" << std::endl;
    std::string str = " a,b,c,d ";
    assert(string::trimmed(str) == true);
    assert(str == "a,b,c,d");
    assert(string::trimmed(str) == true);
    assert(str == "a,b,c,d");
    str = "\ta,b,c,d\t";
    assert(string::trimmed(str) == true);
    assert(str == "a,b,c,d");
    str = "\na,b,c,d\n";
    assert(string::trimmed(str) == true);
    assert(str == "a,b,c,d");
    str = "\ra,b,c,d\r";
    assert(string::trimmed(str) == true);
    assert(str == "a,b,c,d");
    std::cout << "test_trimmed OK" << std::endl;
}

void test_string_toLower()
{
    std::cout << "test_toLower" << std::endl;
    std::string str = "a,b,c,d";
    assert(string::toLower(str) == true);
    assert(str == "a,b,c,d");
    assert(string::toLower(str) == true);
    assert(str == "a,b,c,d");
    str = "A,b,c,d";
    assert(string::toLower(str) == true);
    assert(str == "a,b,c,d");
    str = "a,B,c,d";
    assert(string::toLower(str) == true);
    assert(str == "a,b,c,d");
    str = "a,b,C,d";
    assert(string::toLower(str) == true);
    assert(str == "a,b,c,d");
    str = "a,b,c,D";
    assert(string::toLower(str) == true);
    assert(str == "a,b,c,d");
    std::cout << "test_toLower OK" << std::endl;
}

void test_string_toUpper()
{
    std::cout << "test_toUpper" << std::endl;
    std::string str = "a,b,c,d";
    assert(string::toUpper(str) == true);
    assert(str == "A,B,C,D");
    std::cout << "test_toUpper OK" << std::endl;
}

void test_string_startsWith()
{
    std::cout << "test_string_startsWith" << std::endl;
    std::string text = "abcdef";
    std::string str = "abc";
    assert(string::startsWith(text, str));
    str = "def";
    assert(!string::startsWith(text, str));
    std::cout << "test_string_startsWith OK" << std::endl;
}

void test_string_endsWith()
{
    std::cout << "test_string_endsWith" << std::endl;
    std::string text = "abcdef";
    std::string str = "def";
    assert(string::endsWith(text, str));
    str = "abc";
    assert(!string::endsWith(text, str));
    std::cout << "test_string_endsWith OK" << std::endl;
}

void test_string_contains()
{
    std::cout << "test_string_contains" << std::endl;
    std::string text = "abcdef";
    std::string str = "def";
    assert(string::contains(text, str));
    str = "ac";
    assert(!string::contains(text, str));
    std::cout << "test_string_contains OK" << std::endl;
}

void test_string_isLower()
{
    std::cout << "test_isLower" << std::endl;
    std::string str = "a";
    assert(string::isLower(str));
    str = "A";
    assert(!string::isLower(str));
    std::cout << "test_isLower OK" << std::endl;
}

void test_string_isUpper()
{
    std::cout << "test_isUpper" << std::endl;
    std::string str = "A";
    assert(string::isUpper(str));
    str = "a";
    assert(!string::isUpper(str));
    std::cout << "test_isUpper OK" << std::endl;
}

int main()
{
    test_string_split();
    test_string_remove();
    test_string_replace();
    test_string_trimmed();
    test_string_toLower();
    test_string_toUpper();
    test_string_startsWith();
    test_string_endsWith();
    test_string_contains();
    test_string_isLower();
    test_string_isUpper();
    return 0;
}
