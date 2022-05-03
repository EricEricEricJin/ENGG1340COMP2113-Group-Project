#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

void f()
{
    for (int i = 0; i < 10; i++)
    {
        cout << "f " << i << endl;
        sleep(1);
    }
}

void g()
{
    for (int i = 0; i < 10; i++)
    {
        cout << "g " << i << endl;
        sleep(1);
    }
}

int main()
{
    thread t1(f);
    thread t2(g);
    t1.join();
    t2.join();
    return 0;
}


