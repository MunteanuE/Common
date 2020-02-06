#include "cstack.h"
#ifdef CSTACK_H

template class Stack<uint64_t>;
template class Dynamic_stack<uint64_t>;

template <class X>
inline void Stack<X>::push(X val)
{
    if( size != tos)
    {
        *(head + tos) = val;
        tos++;
    } else
    {
        throw 1;
    }
}

template <class X>
inline X Stack<X>::pop(void)
{
    if(!isEmpty())
    {
        tos--;
        return *(head + tos);
    } else
    {
        throw 2;
    }
}

template <class X>
inline X Stack<X>::top(void)
{
    if(!isEmpty())
    {
        return *(head + tos - 1);
    } else
    {
        throw 3;
    }
}

template <class X>
inline void Stack<X>::addRandomValue(void)
{
    try
    {
        push(rand());
    }
    catch(int i)
    {
        std::cerr<<"Operation push interrupted: stack overflow, code of error "<<i<<std::endl;
    }
}

template <class X>
inline void Stack<X>::printData(void)
{
    try
    {
        std::cout<<top()<<std::endl;
    }
    catch(int i)
    {
        std::cerr<<"Operation top interrupted: stack end, code of error "<<i<<std::endl;
    }
}

template <class X>
inline X Stack<X>::getData(void)
{
    try
    {
        return pop();
    }
    catch(int i)
    {
        std::cerr<<"Exit program. Operation pop interrupted: stack end, code of error "<<i<<std::endl;
        delete [] head;
        exit(i);
    }
}

template <class X>
inline Stack<X>::Stack(uint64_t size_stack)
{
	try
	{
		head = new X[size_stack];
	}
	catch(std::bad_alloc& ba)
	{
		std::cerr<<"bad allocation stack"<<std::endl;
		exit(1);
	}
	this->size=size;
	tos = 0;
	srand(clock());
}

template <class X>
void Dynamic_stack<X>::push(X val)
{
    if( Stack<X>::size != Stack<X>::tos )
    {
        *(Stack<X>::head + Stack<X>::tos) = val;
        Stack<X>::tos++;
    } else
    {
        Stack<X>::size += 16;

        try
        {
            uint64_t *temp = new X[Stack<X>::size];
            memcpy(temp, Stack<X>::head, Stack<X>::tos*sizeof(X));
            delete [] Stack<X>::head;
            Stack<X>::head = temp;
        }
        catch(std::bad_alloc& ba)
        {
            std::cerr<<"bad allocation stack"<<std::endl;
            exit(1);
        }

        *(Stack<X>::head+Stack<X>::tos)=val;
        Stack<X>::tos++;
    }
}

#endif
