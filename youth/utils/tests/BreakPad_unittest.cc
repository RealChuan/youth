#include <youth/utils/BreakPad.hpp>

#include <string>

using namespace youth::utils;

class Test
{
public:
    Test()
        : m_sum(new int(1))
    {
        printf("Test\n");
    }
    ~Test()
    {
        delete m_sum;
        printf("~Test\n");
    }

    void print() { printf("print %d\n", *m_sum); }

private:
    int *m_sum;
};

int main(int argc, char **argv)
{
    BreakPad bp(".");

    auto test = new Test;
    delete test;
    delete test;
    test->print();

    return 0;
}
