#include "../String.h"
#include "../LogOut.h"

using namespace youth;

int main(){
    String str = "/yyy/xxx/zzz";
    std::vector<String> strVec = str.split('/');
    for(const String& s : strVec)
        LOG_INFO << s;
    return 0;
}
