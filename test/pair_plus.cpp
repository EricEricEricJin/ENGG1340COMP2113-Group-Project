#include <iostream>
using namespace std;

int main()
{
    pair<int, int> a {1, 2};
    a = a + make_pair(3, 4);
    cout << a.first << " " << a.second << endl;
    return 0;
}