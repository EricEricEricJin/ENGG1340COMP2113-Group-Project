#include <iostream>
#include <vector>
#include <list>

using namespace std;

int main()
{
    list<int> l = {1, 2, 3, 4, 5};
    auto l_it = l.begin();
    l.erase(l_it++);
    cout << *l_it << endl;

    for (auto& v : l)
        cout << v << " ";
    cout << endl;

    return 0;
}