#ifndef PROCESSMSG_H
#define PROCESSMSG_H

#include <string>

namespace youth
{

namespace ProcessMsg
{

pid_t getTid();
pid_t getPid();
std::string hostname();
std::string fileBaseName(const char*);

}

}

#endif /* PROCESSMSG_H */
