#include <panel.h>
#include <cstring>

#define NLINES 10
#define NCOLS 40

int main()
{
    WINDOW *my_wins[3];
    PANEL *my_panels[3];
    PANEL *top;
    int ch;

    /* Initialize curses */
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    /* Initialize all the colors */
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);

    init_wins(my_wins, 3);

    /* Attach a panel to each window */   /* Order is bottom up */
    my_panels[0] = new_panel(my_wins[0]); /* Push 0, order: stdscr-0 */
    my_panels[1] = new_panel(my_wins[1]); /* Push 1, order: stdscr-0-1 */
    my_panels[2] = new_panel(my_wins[2]); /* Push 2, order: stdscr-0-1-2 */

    /* Set up the user pointers to the next panel */
    set_panel_userptr(my_panels[0], my_panels[1]);
    set_panel_userptr(my_panels[1], my_panels[2]);
    set_panel_userptr(my_panels[2], my_panels[0]);

    /* Update the stacking order. 2nd panel will be on top */
    update_panels();

    /* Show it on the screen */
    attron(COLOR_PAIR(4));
    mvprintw(LINES - 2, 0, "Use tab to browse through the windows (F1 to Exit)");
    attroff(COLOR_PAIR(4));
    doupdate();

    top = my_panels[2];
    while ((ch = getch()) != KEY_F(1))
    {
        switch (ch)
        {
        case 9:
            top = (PANEL *)panel_userptr(top);
            top_panel(top);
            break;
        }
        update_panels();
        doupdate();
    }
    endwin();
    return 0;
}

