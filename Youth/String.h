#ifndef STRING_H
#define STRING_H

#include <string>
#include <vector>

namespace youth {

class String : public std::string
{
public:
    explicit String();
    explicit String(const char*);
    explicit String(const std::string&);

    String &operator=(const char*);
    String &operator=(const std::string&);

    std::vector<String> split(const char);

    //std::string toString();
};

}

#endif // STRING_H
