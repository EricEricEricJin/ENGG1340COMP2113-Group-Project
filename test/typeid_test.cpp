#include <iostream>
using namespace std;

template <typename T>
bool f(T x)
{
    return typeid(T) == typeid(string);
}

int main()
{
    cout << f(string("AA")) << endl;
    return 0;
}