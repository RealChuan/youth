#include "../ElapsedTime.h"
#include "../LogOut.h"

#include <unistd.h>

using namespace youth;

int main(int argc, char** argv){
    Logging::setFileBaseName(argv[0]);
    Logging::setOutputMode(LOGGER_MODE_LOGFILE);
    ElapsedTime timer;
    timer.start();
    for (int i = 0; i < 1000 * 1000; i++)
        LOG_INFO << i;
    printf("%s\n", timer.elapsed().c_str());
    return 0;
}
