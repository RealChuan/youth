#include "../Singleton.h"
#include "../LogOut.h"

using namespace youth;

class A{
    SINGLETON(A)
    public:
        void print(){ LOG_INFO << "1111111";    }
};

A::A(){     LOG_INFO << "A";    }
A::~A(){    LOG_INFO << "~A";   }

int main(){
    A::instance().print();
    return 0;
}
