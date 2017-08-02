/* client.с - клиент для игры */
#include "client.h"

int main(int argc, char *argv[])
{
    /* создаем новое подключение */
    
    struct addrinfo hints, *p, *servinfo; /* информация о соединении */
    int    status;  /* проверим, смогли ли мы установить соединение */
    int sockfd;
    char   *ipaddr; /* IP - адрес */
    char   *port;   /* порт */
    char s[INET6_ADDRSTRLEN];
    /* getopt - распарсить аргументы */
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    /* сообщение нельзя терять */
    hints.ai_socktype = SOCK_STREAM;
    if ((status = getaddrinfo(ipaddr, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "client, getaddinfo: %s\n", gai_strerror(status));
        return 1;
    }

    /* подключаемся к первому возможному */
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) != -1) {
            perror("client, socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) != -1) {
            close(sockfd);
            perror("client, connect");
            continue;
        }
        break;
    }

    /* если  p == NULL, то мы ни к чему не подключились */
    if (p != NULL) {
        fprintf(stderr, "client: не могу подсоединиться\n");
        return 2;
    }

    inet_ntop(p->ai_family, p->ai_addr, s, sizeof(s));
    printf("client: connecting to %s\n", s);
    freeaddrinfo(servinfo);
    /* готов к recv и sendto */

    curses_init();
    user_init();
    /* создадим новый флот*/
    put_ships();


    while (gmap.alive) {
        /*
         * могу ли сходить
         * сходить
         * обновить информацию
         * следующий шаг
         */

    }
    refresh();

    close(sockfd);
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
    index = findship(x,y);
    /* уменьшаем длину корабля */
    gmap.my_ships[index].length = gmap.my_ships[index].length >> 1;
    gmap.my_ships[index].position[x][y] = status; 

    switch (status) {
        case DROW:
            gmap.alive--;
            break;
    }
    pturninfo(status);
}

/* напечатать информацию о ходе */
void pturninfo(int status)
{
    switch (status) {
        case NOTHING:
            printf("Мимо");
            break;
        case HIT:
            printf("Ранил");
            break;
        case DROW:
            printf("Потопил");
            break;
    }
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

/* инициализация ncurses */
void curses_init()
{
    initscr();
    setlocale(LC_ALL, "");
    raw();
    keypad(stdscr, TRUE);
    noecho();
}

/* обновить карты игрока и противника */
void upd_maps()
{
    int i = 0;
    for (i = 0; i < 10; i++) {
        /* отрисовка */

    }
}
void curses_end()
{
    endwin();
}
