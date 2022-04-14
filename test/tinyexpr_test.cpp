#include "tinyexpr.h"
#include <iostream>

using namespace std;

int main()
{
    double a = 11;
    te_variable vars[] = {{"a", &a}};
    te_expr *n = te_compile("a+2", vars, 1, 0);
    cout << te_eval(n) << endl;
    return 0;
}