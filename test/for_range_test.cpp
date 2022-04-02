#include <iostream>
#include <vector>

using namespace std;

class A
{
private:
    int v = 0;
public:
    void add()
    {
        v += 1;
    }
    int get()
    {
        return v;
    }
};


int main()
{
    A a;
    vector<A> l = {a};   
    cout << l[0].get() << endl;
    for (int i = 0; auto &a : l; i++)
        a.add();
    cout << l[0].get() << endl;
    return 0;
}