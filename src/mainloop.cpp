#include "mainloop.h"

void main_loop()
{
    Map M(WIN_H, WIN_W);
    Screen S(10);

    int bird_y;
    bird_y = WIN_H / 2;
    S.set_bird_y(&bird_y);
    S.set_map(M._map._map, M._map._map_w, M._map._map_h);

    int score = 0;

    while (true)
    {
        M.setup();
        for (int i = 0;; i++)
        {
            char key = getch();
            switch (key)
            {
            case KEY_UP:
                if (bird_y > 0)
                    bird_y -= 1;
                break;
            case KEY_DOWN:
                if (bird_y < WIN_H - 1)
                    bird_y += 1;
                break;
            default:
                break;
            }

            if (i % 100 == 0)
                M.step_fwd();

            if (i % 10 == 0)
                score += 1;

            S.update(score);

            if (M._map._map[bird_y][10] == 1)
                break;

            usleep(1000);
        }

        S.die();
        sleep(1);

        // endwin();
    }
}