#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std;

int main()
{
    string user_folder = getenv("HOME");
    cout << user_folder << endl;

    ifstream f("HOME/testfile.txt"); 
    if (f.fail())
    {
        cout << "Error" << endl;
    }
    else
    {
        int a;
        f >> a;
        cout << a << endl;
    }
    f.close();
    return 0;
}