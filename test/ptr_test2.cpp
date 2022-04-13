#include "ptr_test2.h"
#include "ptr_test1.h"
void f(void* v)
{
    cout << ((A*)v)->x << endl;
}