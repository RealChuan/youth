#ifndef STRING_H
#define STRING_H

#include <string>
#include <vector>

namespace youth {

class String : public std::string
{
public:
    String();
    String(const char*);
    String(std::string);

    std::vector<String> split(const char);

    //std::string toString();
};

}

#endif // STRING_H
