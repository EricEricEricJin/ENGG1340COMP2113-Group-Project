#include <iostream>
#include "setting.hpp"

using namespace std;

int main()
{
    game::Setting setting;
    cout << "load success: " << setting.load() << endl;
    cout << setting.query<int>("key") << endl;
    return 0;
}