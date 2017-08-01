/* client.h */
#ifndef CLIENT_H
#define CLINET_H
#include <locale.h>
#include <wctype.h>
#include <wchar.h>
#include <ncurses.h>
#include "map.h"
/* ncurses не хочет работать с Unicode - символами, приходится извращаться */

const wchar_t *dot = L"\u2022"; /* • */
const wchar_t *xmark = L"\u00d7"; /* × */
const char hbar[] = "абвгдежзкик"; /* горизонтальный ряд */

static struct map gmap;
void curses_init();
void curses_end();

void fire(int x, int y);
void user_init();
void upd_user_field(int x, int y, int status);
void upd_enemy_field(int x, int y, int status);

#endif
