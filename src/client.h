/* client.h */
#ifndef CLIENT_H
#define CLINET_H
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
/* ncurses не хочет работать с Unicode - символами, приходится извращаться */

const wchar_t *dot = L"\u2022"; /* • */
const wchar_t *xmark = L"\u00d7"; /* × */
const char hbar[] = "абвгдежзкик"; /* горизонтальный ряд */

static struct map gmap;
void curses_init();
void curses_end();

void fire(int, int);
void user_init();
void upd_user_field(int, int, int);
void upd_enemy_field(int, int, int);
void upd_maps();

void pturninfo(int status);
void put_ships();
int findship(int, int);
#endif
