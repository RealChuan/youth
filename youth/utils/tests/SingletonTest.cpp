#include <youth/utils/Logging.h>
#include <youth/utils/Singleton.h>

using namespace youth::utils;

class S {
  SINGLETON(S)
public:
  void print() { LOG_INFO << "1111111"; }
};

S::S() { LOG_INFO << "A"; }
S::~S() { LOG_INFO << "~A"; }

int main() {
  S::instance().print();
  S::instance().print();
  S::instance().print();
  S::instance().print();
  return 0;
}
