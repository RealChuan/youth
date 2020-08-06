#include "../FileUtil.h"

using namespace youth::core;

int main()
{
    std::string filename = "xxx/yyy/zzz.txt";
    FileUtil file;
    file.setFileName(filename);
    file.open(FileUtil::ReadAndWrite);
    for(int i=0; i<1000; i++){
        file.write("123456789\n");
        file.write("987654321\n");
    }
    file.flushFile();
    //LOG_INFO << file.readLine() << file.readAll();
    return 0;
}
