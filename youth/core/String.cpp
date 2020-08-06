#include "String.h"

namespace youth
{

namespace core
{

std::vector<std::string> string::split(std::string str, const char s)
{
    std::vector<std::string> result;
    size_t start = 0;
    size_t pos = str.find(s);
    while (pos != std::string::npos)
    {
        if (pos > start)
        {
            result.push_back(str.substr(start, pos-start));
        }
        start = pos + 1;
        pos = str.find(str, start);
    }

    if (start < str.length())
    {
        result.push_back(str.substr(start));
    }

    return result;
}

}

}
