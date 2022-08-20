#include "Process.hpp"
#include "File.hpp"

#include <signal.h>
#include <sys/syscall.h>
#include <unistd.h>

extern char **environ;

namespace youth {

namespace core {

namespace Process {

pid_t getTid()
{
    return ::syscall(SYS_gettid);
}

pid_t getPid()
{
    return ::getpid();
}

std::string hostname()
{
    char buf[256];
    if (::gethostname(buf, sizeof(buf)) == 0) {
        return buf;
    }
    return "";
}

std::string fileBaseName(const std::string &basename)
{
    std::string name = basename;
    size_t pos = name.find_last_of('/');
    if (pos != std::string::npos) {
        return name.substr(pos + 1);
    }
    return name;
}

int excute(const std::string &cmd)
{
    return ::system(cmd.c_str());
}

bool kill(pid_t pid)
{
    if (pid <= 0) {
        return false;
    }
    return ::kill(pid, SIGKILL) == 0;
}

bool kill(const std::string &name)
{
    return kill(getPidFromName(name));
}

pid_t getPidFromName(const std::string &name)
{
    pid_t pid = 0;
    std::string cmd = "pidof " + name + " > ./pid.txt";
    int ret = excute(cmd);
    if (ret != 0) {
        return pid;
    }
    File file("./pid.txt");
    if (!file.open(File::OpenMode::ReadOnly)) {
        return pid;
    }
    std::string line;
    while (!file.atEnd()) {
        line = file.readLine();
        pid = std::stoi(line);
        break;
    }
    file.close();
    file.remove();
    return pid;
}

std::string getCwd()
{
    char buf[1024];
    if (::getcwd(buf, sizeof buf) == nullptr) {
        return "unknowncwd";
    } else {
        return buf;
    }
}

std::vector<std::string> systemEnvironment()
{
    std::vector<std::string> env;
    char **s = environ;
    for (; *s; s++) {
        env.push_back(*s);
    }
    return env;
}

bool setEnvironment(const std::string &name, const std::string &value)
{
    return ::setenv(name.c_str(), value.c_str(), 1) == 0;
}

std::string getEnvironment(const std::string &name)
{
    char *env = ::getenv(name.c_str());
    if (env == nullptr) {
        return "";
    } else {
        return env;
    }
}

File nullDevice()
{
    return File("/dev/null");
}

} // namespace Process

} // namespace core

} // namespace youth
