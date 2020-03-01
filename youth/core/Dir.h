#ifndef DIR_H
#define DIR_H

#include <string>

namespace youth {

namespace Dir {
void makeDirectory(const std::string&); //新建单个目录
void newDirectory(const std::string&);  //新建多级目录
void getCurrenPath();
std::string currentPath();

}

}

#endif // DIR_H
