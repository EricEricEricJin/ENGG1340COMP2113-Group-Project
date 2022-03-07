#include "map.h"

int main()
{
    Map M(10, 100);

    for (int i = 0; i < 100; i++)
    {
        M.step_fwd();
        printf("START\n");
        for (int row = 0; row < M._map._map_h; row++)
        {
            for (int col = 0; col < M._map._map_w; col++)
            {
                if (M._map._map[row][col] == 0)
                    printf(" ");
                else
                    printf("*");
            }
            printf("\n");
        }
    }
    return 0;
}