#ifndef CSTACK_H
#define CSTACK_H
#include "cbench_data.h"
#include <cstdint>
#include <iostream>
#include <string.h>

template <class X>
class Stack : public Bench_data<X>
{
    protected:
	X *head = nullptr;
	uint64_t size = 0;
	uint64_t tos = 0;
    virtual void push(X val);
	X pop(void);
	X top(void);
    ~Stack(){if( nullptr != head) delete [] head;};

	public:
	Stack(uint64_t size);
	bool isEmpty(void){return tos > 0 ? 0 : 1;};
	void addRandomValue(void);
	void printData(void);
    X getData(void);
};

template <class X>
class Dynamic_stack : public Stack<X>
{
    protected:
    void push(X val);
	public:
    Dynamic_stack(uint64_t size_dstack) : Stack<X>(size_dstack){};
};

#endif //CSTACK_H
