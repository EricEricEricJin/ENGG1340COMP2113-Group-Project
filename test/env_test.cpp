#include <iostream>
using namespace std;

int main()
{
    string s = getenv("HOME");
    cout << s << endl;
    return 0;
}