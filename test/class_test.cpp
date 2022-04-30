#include <iostream>

using namespace std;

class A
{
private:
    int a = 10;
public:
    void f(int a)
    {
        cout << a << " " << this->a;
    }
};

int main()
{
    A a;
    a.f(20);
    return 0;
}
