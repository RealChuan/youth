#include "../String.h"

using namespace youth::core;

int main()
{
    std::string str("/yyy/xxx/zzz.log");
    std::vector<std::string> strVec = string::split(str, '/');
    for(const std::string& s : strVec)
        printf("%s\n", s.c_str());
    return 0;
}
