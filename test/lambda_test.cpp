#include <iostream>
#include <map>
#include "tinyexpr.h"

using namespace std;

map<int, bool (*)(int, int)> m;

int main()
{
    auto f = [](int a, int b)
    { return a > b; };
    m[0] = &f;
    (*m[0])(1, 2);
    return 0;
}