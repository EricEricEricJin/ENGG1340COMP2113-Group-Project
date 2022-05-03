#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

using namespace std;

mutex m;
condition_variable cv;
int t = 0;

void f1()
{
    unique_lock lk(m);
    cv.wait(lk, [&t]
            { return (t > 5); });
    cout << "Done" << endl;
}

int main()
{
    thread t1(f1);
    for (int i = 0; i < 100; i++)
    {
        sleep(1);
        t++;
    }
    t1.join();
    return 0;
}