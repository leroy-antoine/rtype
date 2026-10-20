#pragma once

#include <iostream>
#include <string>
#include <vector>

// Mini harnais de tests : pas de dependance externe.
namespace test {
    using TestFn = void (*)();

    struct Case {
        std::string name;
        TestFn run;
    };

    inline std::vector<Case> &cases()
    {
        static std::vector<Case> all;

        return all;
    }

    inline int &failures()
    {
        static int count = 0;

        return count;
    }

    struct Registrar {
        Registrar(const std::string &name, const TestFn run)
        {
            cases().push_back({name, run});
        }
    };
}

#define TEST(name) \
    static void name(); \
    static const test::Registrar registrar_##name(#name, name); \
    static void name()

#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << __FILE__ << ":" << __LINE__ \
                << ": CHECK failed: " #condition << std::endl; \
            ++test::failures(); \
        } \
    } while (false)
