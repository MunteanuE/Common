#ifndef BENCH_DATA_H
#define BENCH_DATA_H
#include <time.h>
#include <stdio.h>

template <class X>
class Bench_data
{
	double time_start = 0;
	double time_end = 0;
	double time_elapsed = 0;
	void transformClock(void) {time_elapsed = time_end - time_start;};

	public:
	void startClock(void) {time_start = (double)clock() / (double)CLOCKS_PER_SEC;};
	void endClock(void) {time_end = (double)clock() / (double)CLOCKS_PER_SEC;};
	void printClock(void) {transformClock(); printf("CPU TIME: %.2f sec\n", time_elapsed);};
	virtual void addRandomValue() = 0;
    virtual void printData() = 0;
    virtual X getData() = 0;
};

#endif //BENCH_DATA_H
