/* client.с - клиент для игры */
#include "client.h"

int main(int argc, char *argv[])
{
    /* создаем новое подключение */

    struct addrinfo hints, *p, *servinfo; /* информация о соединении */
    int    status;  /* проверим, смогли ли мы установить соединение */
    int    sockfd;
    char   *ipaddr; /* IP - адрес */
    char   *port;   /* порт */
    char   s[INET6_ADDRSTRLEN];
    /* getopt - распарсить аргументы */


    int opt;

    /* То ли количество аргументов командной строки? */
    if (argc < ARGS_NUM) {
            fprintf(stderr, "Usage: %s [-i ipaddress] [-p portname]\n",
                   argv[0]);
            exit(EXIT_FAILURE);
    }
    /* надо выцепить ip-адрес и порт сервера */
    while ((opt = getopt(argc, argv, "i:p:")) != -1) {
       switch (opt) {
           case 'i':
               ipaddr = optarg;
               break;
           case 'p':
               port = optarg;
           break;
           default: /* ??? */
               fprintf(stderr, "Usage: %s [-i ipaddress] [-p portname]\n",
                       argv[0]);
               exit(EXIT_FAILURE);
       }
    }

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    /* сообщение нельзя  потерять */
    hints.ai_socktype = SOCK_STREAM;
    if ((status = getaddrinfo(ipaddr, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "client, getaddinfo: %s\n", gai_strerror(status));
        return 1;
    }

    /* подключаемся к первому возможному */
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client, socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client, connect");
            continue;
        }
        break;
    }

    /* если  p == NULL, то мы ни к чему не подключились */
    if (p == NULL) {
        fprintf(stderr, "client: не могу подсоединиться\n");
        return 2;
    }

    inet_ntop(p->ai_family, p->ai_addr, s, sizeof(s));
    printf("client: connecting to %s\n", s);
    freeaddrinfo(servinfo);
    /* готов к recv и sendto */

#if 0
    curses_init();
    user_init();
    refresh();
    /* создадим новый флот*/
    put_ships();
    /* send ("готов")
     * recv ("другой игрок готов");
     */
    while (gmap.alive) {

         * recv ("твой ход") или recv("ход противника")
         * scanf(%c %d, &x, &y);
         * ynum = pos_by_num(x);
         * send(x,y)
         * recv(status)
         * upd_enemy_field();
         * redraw();
         * send(ход закончил)

    }
    mvwprintw(stdscr, MSGPOS, 0, "Конец игры");
    refresh();
    getch();
    curses_end();
#endif
    close(sockfd);
    return 0;
}

/* обновляем данные игрока, когда стреляют по нему */
void upd_user_field(int x, int y, int status)
{
    int index;
    gmap.my_field[x][y] = status;
    index = findship(x,y);
    /* уменьшаем длину корабля */
    gmap.my_ships[index].length = gmap.my_ships[index].length >> 1;
    gmap.my_ships[index].position[x][y] = status; 

    switch (status) {
        case DROW:
            gmap.alive--;
            break;
    }
    penemyinfo(status);
}

/* дублирую, но пофиг */
void penemyinfo(int status)
{
    switch (status) {
        case MISS:
            mvwprintw(stdscr, MSGPOS, 0, "Противкник промазал");
            break;
        case HIT:
            mvwprintw(stdscr, MSGPOS, 0, "Противник повредил корабль");
            break;
        case DROW:
            mvwprintw(stdscr, MSGPOS, 0, "Противник потопил корабль");
            break;
    }
}

/* напечатать информацию */
void pinfo(int status)
{
    switch (status) {
        case MISS:
            mvwprintw(stdscr, MSGPOS, 0, "Мимо");
            break;
        case HIT:
            mvwprintw(stdscr, MSGPOS, 0, "Ранил");
            break;
        case DROW:
            mvwprintw(stdscr, MSGPOS, 0, "Потопил");
            break;
        case ERROR_PUT:
            mvwprintw(stdscr, MSGPOS, 0, "Не могу поставить корабль");
            break;
    }
}

/* обновляем данные игрока, когда стреялет он */
void upd_enemy_field(int x, int y, int status)
{
    gmap.en_field[x][y] = status;
    pinfo(status);
}

void user_init()
{
    memset(&gmap, 0, sizeof(gmap));
}

/* инициализация ncurses */
void curses_init()
{
    setlocale(LC_ALL, "");
    initscr();
    raw();
    keypad(stdscr, TRUE);
}

/* обновить карты игрока и противника */
void upd_maps() {}

void curses_end()
{
    endwin();
}

/* есть ли символ в наборе */
int inset(char x)
{
    int i;
    for (i = 0; i < MAPSIZE; i++)
        if(hbar[i] == x)
            return 1;
    return 0;
}

/* определить место в массиве */
int getx(char x)
{
    int i;
    for (i = 0; i < MAPSIZE; i++)
        if(hbar[i] == x)
            return i;
    return -1;
}

/* проверяю, могу ли поставить корабль на эту позицию */
int canput(int x, int y, int j)
{

    /*
     *     # - часть корабля
     *     @ - куда могу поставить часть
     *
     *     Для начала надо проверить, нет ли корабля на этой клетке
     *
     *
     *     @
     *    @#@   - так мы можем поставить новую "палубу"
     *     @
     *
     *     Игрок может попытаться поставить другую часть в отдалении
     *     от основной части корабля:
     *
     *     @
     *
     *
     *     ##
     *
     *     делаем проверку этого условия и в случае успеха
     *     cross = 1
     *
     *     Может получиться так, что игрок будет создавать корабль
     *     как-то так:
     *
     *    @ @
     *     #
     *    @#@
     *
     *     Для этого делаем последнюю проверку
     */
    if (gmap.my_field[x][y] == SHIP)
        return 0;
    if (!((x > 0 && gmap.my_field[x - 1][y] == SHIP) ||
       (y > 0 && gmap.my_field[x][y - 1] == SHIP) ||
       (x < MAPSIZE - 1 && gmap.my_field[x + 1][y] == SHIP) ||
       (y < MAPSIZE - 1 && gmap.my_field[x][y + 1] == SHIP)) && j !=0)
             return 0;
    if ((x > 0 && y > 0 &&gmap.my_field[x - 1][y - 1] == SHIP) ||
        (x<MAPSIZE-1 && y<MAPSIZE-1 && gmap.my_field[x+1][y+1] == SHIP) ||
        (x < MAPSIZE-1 && y >0 && gmap.my_field[x+ 1][y - 1] == SHIP) ||
        (x > 0 && y < MAPSIZE-1 &&gmap.my_field[x - 1][y + 1] == SHIP))
        return 0;
    return 1;
}


/* расставляем корабли*/
void put_ships()
{
    int i, j, k;
    char xchar;
    char *data;
    int x;
    int y;
    int ships = 1;
    int size = MAXSHIPSIZE;
    int offset = 0;
    int yes = 0;
    int tmp;
    char str[STRLEN];
    struct cell input[MAXSHIPSIZE];
    /* всего нужно поставить SHIPSNUM кораблей */
    for (i = 0; i < SHIPSNUM; ) {
        for (j = 0; j < ships;) { /* каждого корабля - ships штук*/
            yes= 0;
            wmove(stdscr, MSGPOS + 2,0);
            wclrtoeol(stdscr);
            mvwprintw(stdscr, MSGPOS  +1, 0, "Введите координаты ");
            refresh();
            mvwgetstr(stdscr, MSGPOS +2, 0, str);
            offset = 0;
            data = str;
            /* у каждого корабля size частей */
            for (k = 0; k < size; k++) {
                if ((tmp = sscanf(data,"%c%d%n",&xchar,&y,&offset))== 2) {
                    data += offset;
                    y--;
                    if (inset(xchar) && y >= 0 && y < 10) {
                        x = getx(xchar);
                        if(canput(x,y,j)){
                            input[k].x = x;
                            input[k].y = y;
                            wclear(stdscr);
                            yes = 1;
                            i++;
                        } else {
                            /* ошибка*/
                            pinfo(ERROR_PUT);
                            yes = 0;
                            break;
                        }
                    } else {
                        /* ошибка*/
                        pinfo(ERROR_PUT);
                        yes = 0;
                        break;
                    }
                } else {
                    /* ошибка*/
                    pinfo(ERROR_PUT);
                    yes = 0;
                    break;
                }
            }
            if(yes) {
                for (k = 0; k < size; j++){
                    gmap.my_field[input[k].x][input[k].y] = SHIP;
                    gmap.my_ships[i].position[input[k].x][input[k].y] = SHIP;
                }
                j++;
            }
        }
        ships++;
        size--;
    }

}

/* найти корабля по координатам */
int findship(int x, int y)
{
    int i;
    for (i = 0; i < SHIPSNUM; i++ )
        if (gmap.my_ships[i].position[x][y] == SHIP)
            return i;
    return -1;
}
