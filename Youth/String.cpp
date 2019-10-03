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

String::String(std::string str)
{
    swap(str);
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
            result.push_back(substr(start, pos-start));
        }
        start = pos + 1;
        pos = find(str, start);
    }

    if (start < length())
    {
        result.push_back(substr(start));
    }

    return result;
}

//std::string String::toString()
//{
//    return std::string(c_str());
//}
