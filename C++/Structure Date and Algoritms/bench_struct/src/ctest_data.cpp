#include "ctest_data.h"
#ifdef CTEST_DATA_H

Test_data::Test_data(uint8_t type_data, uint64_t size_stack)
{
    Bench_data<uint64_t> * data;

    switch(type_data)
    {
    case 0:
        data = new Stack<uint64_t>(size_stack);
    break;
    case 1:
        data = new Dynamic_stack<uint64_t>(size_stack);
    break;
    default:;
    }

    data->startClock();

	uint64_t cycles1 = rdtsc();//

	for(uint64_t i = 0; i<size_stack + 1; i++)
    {
        data->addRandomValue();
    }

	for(uint64_t i = 0; i<size_stack + 1; i++) //add 1 for test exception
    {
        data->printData(); //comment it if size of structure too much
        data->getData();
    }

	uint64_t cycles2 = rdtsc();//

	data->endClock();

	uint64_t cycles = cycles2 - cycles1;
	double cpe = (double)cycles / (double)size_stack;
	printf("CYCLES:      %.2lu\n", cycles);
	printf("CPE:      %.2f\n", cpe);

	data->printClock();

	delete data;
}

Test_data::~Test_data()
{

}

inline uint64_t Test_data::rdtsc(void)
{
    uint32_t hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
}

#endif
