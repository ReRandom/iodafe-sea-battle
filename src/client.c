/* client.с - клиент для игры */
#include "client.h"

int main(int argc, char *argv[])
{

    curses_init();
    refresh();
    curses_end();
    return 0;
}

void fire(int x, int y)
{

    if (gmap.en_field[x][y] != NOTHING)
        shoot_again();
    /*
     * send...
     */
}

/* обновляем данные игрока, когда стреляют по нему */
void upd_user_field(int x, int y, int status)
{
    int index;
    gmap.my_field[x][y] = status;
    switch (status) {
        case HIT:
        case DROW:
            index = findship(x,y);
            /* уменьшаем длину корабля */
            gmap.my_ships[index].length = gmap.my_ships[index].length >> 1;
            gmap.my_ships[index].position[x][y] = status; 
            break;
    }
    pturninfo(status);
}

/* обновляем данные игрока, когда стреялет он */
void upd_enemy_field(int x, int y, int status)
{
    gmap.en_field[x][y] = status;
    pturninfo(status);
}

void user_init()
{
    memset(&gmap, 0, sizeof(gmap));
}
void curses_init()
{
    initscr();
    setlocale(LC_ALL, "");
    raw();
    keypad(stdscr, TRUE);
    noecho();
}

void curses_end()
{
    endwin();
}
