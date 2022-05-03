#include <iostream>

#include "bullet.h"
#include "player.h"
#include "zombie.h"
#include "map.h"

using namespace std;

int main()
{
    game::bulletManager bm;
    
    cout << "Load resource..." << endl;
    cout << "Success: " << bm.load_resource("../resource/") << endl;



    bm.run()
    return 0;
}