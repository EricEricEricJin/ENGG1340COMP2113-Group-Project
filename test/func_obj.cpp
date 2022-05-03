/*
    Cannot pass non-static member function as function object or pointer
*/


#include <iostream>
#include <functional>

using namespace std;

class A
{
private:
    int x;

public:
    static void f(int _x)
    {
        x = _x;
        cout << x << endl;
    }
};

void g(void (*f)(int))
{
    f(1);
}

int main()
{
    A *a = new A;
    g(a->f);
    return 0;
}