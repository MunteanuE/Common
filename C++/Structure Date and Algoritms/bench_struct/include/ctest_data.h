#ifndef CTEST_DATA_H
#define CTEST_DATA_H
#include "cstack.h"

#define STACK 0
#define DSTACK 1

class Test_data
{
    uint64_t size = 0;
    uint8_t type = 0;

    public:
    Test_data(uint8_t type, uint64_t size);
    ~Test_data();
    uint64_t rdtsc(void);
};

#endif // CTEST_DATA_H
