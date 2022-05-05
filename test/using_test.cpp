#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<vector<int>> a = {{1, 2, 3}, {4, 5, 6}};
    using hama = a[1];
    cout << a << endl;
    return 0;
}