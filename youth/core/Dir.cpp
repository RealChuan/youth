#include "Dir.h"
#include "String.h"

#include <sys/stat.h>
#include <unistd.h>

namespace youth {

namespace core {

int currentPathLength;
char currentPath_[32];

void Dir::makeDirectory(const std::string &path)
{
    if (path.empty())
        return;
    if (access(path.c_str(), F_OK) == -1) {
        mkdir(path.c_str(), 0777);
        return;
    }
    //fprintf(stderr, "The path is already exist!");
}

void Dir::newDirectory(const std::string &path)
{
    if (path.empty())
        return;
    std::vector<std::string> fileVec = string::split(path, '/');
    size_t size = fileVec.size() - 1;
    for (size_t i = 0; i < size; i++) {
        makeDirectory(fileVec[i]);
        fileVec[i + 1] = fileVec[i] + "/" + fileVec[i + 1];
    }
}

void Dir::getCurrenPath()
{
    currentPathLength = snprintf(currentPath_, sizeof currentPath_, "%s", get_current_dir_name());
}

std::string Dir::currentPath()
{
    if (currentPathLength == 0)
        getCurrenPath();
    return currentPath_;
}

} // namespace core

} // namespace youth
