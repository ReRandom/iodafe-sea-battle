/* client.h */
#ifndef CLIENT_H
#define CLINET_H
#define _POSIX_C_SOURCE 200112L
#include <locale.h>
#include <wctype.h>
#include <wchar.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "map.h"

#define ARGS_NUM 5
#define STRLEN 256
#define MSGPOS (LINES - 4)
/* ncurses не хочет работать с Unicode - символами, приходится извращаться */

const wchar_t dot[] = {0x2022, L'\0'}; /* • */
const wchar_t xmark[] = {0x00d7, L'\0'}; /* × */
/*
const char hbar[] = "абвгдежзкик"; 
*/

const char hbar[] = "abcdefghklm"; /* горизонтальный ряд */
struct cell{
    int x;
    int y;
};

static struct map gmap;
void curses_init();
void curses_end();

void fire(int, int);
void user_init();
void upd_user_field(int, int, int);
void upd_enemy_field(int, int, int);
void upd_maps();

void pinfo(int status);
void penemyinfo(int status);
void put_ships();
int findship(int, int);
int inset(char x);
#endif
