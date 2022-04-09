#include <map.h>
#include <iostream>

using namespace std;

int main()
{
    game::Map map;
    cout << "Map testing" << endl;
    cout << "Load success: " << map.load("../resource/map/0.json");
    cout << "Cols: " << map.columns() << " Lines: " << map.lines() << endl;

    cout << "Print map" << endl;
    for (int i = 0; i < map.lines(); i++)
    {
        for (int j = 0; j < map.columns(); j++)
        {
            // cout << "i " << i << " j " << j << endl;
            cout << map.get_char(j, i);
        }
        cout << endl;
    }

    auto zb_ent = map.zb_get_rand_ent_xy();
    cout << "Zombie get random entry: " << zb_ent.first << " " << zb_ent.second << endl;

    cout << "Add at x = 2, y = 3" << endl;
    map.add(2, 3, -1);

    cout << "Print map" << endl;
    for (int i = 0; i < map.lines(); i++)
    {
        for (int j = 0; j < map.columns(); j++)
        {
            // cout << "i " << i << " j " << j << endl;
            cout << map.get_char(j, i);
        }
        cout << endl;
    }

    cout << "Remove at x = 5, y = 3" << endl;
    map.remove(5, 3);

    cout << "Print map" << endl;
    for (int i = 0; i < map.lines(); i++)
    {
        for (int j = 0; j < map.columns(); j++)
        {
            // cout << "i " << i << " j " << j << endl;
            cout << map.get_char(j, i);
        }
        cout << endl;
    }

    getchar();
    return 0;
}