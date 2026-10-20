#include "TestHarness.hpp"

int main()
{
    for (const test::Case &c : test::cases()) {
        const int before = test::failures();

        c.run();
        std::cout << (test::failures() == before ? "[ OK ] " : "[FAIL] ")
            << c.name << std::endl;
    }
    std::cout << test::cases().size() << " tests, " << test::failures()
        << " failed checks" << std::endl;
    return test::failures() == 0 ? 0 : 1;
}
