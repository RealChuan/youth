#include "String.h"

using namespace youth;

String::String()
{

}

String::String(const char *ch)
{
    std::string str = ch;
    swap(str);
}

String::String(const std::string &str)
{
    std::string str_ = str;
    swap(str_);
}

String &String::operator=(const char *ch)
{
    std::string str = ch;
    swap(str);
    return *this;
}

String &String::operator=(const std::string &str)
{
    return operator=(str.c_str());
}

std::vector<String> String::split(const char str)
{
    std::vector<String> result;
    size_t start = 0;
    size_t pos = find(str);
    while (pos != std::string::npos)
    {
        if (pos > start)
        {
            String str_;
            str_ = substr(start, pos-start);
            result.push_back(str_);
        }
        start = pos + 1;
        pos = find(str, start);
    }

    if (start < length())
    {
        String str_1;
        str_1 = substr(start);
        result.push_back(str_1);
    }

    return result;
}

//std::string String::toString()
//{
//    return std::string(c_str());
//}
