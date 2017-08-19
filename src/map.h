/* map.h - тут хранится информация о карте */

#ifndef MAP_H_
#define MAP_H

#include "shot.h"

#define SHIPSNUM 10
#define MAPSIZE 10
#define MAXSHIPSIZE 4

enum message {NOTHING, MISS, HIT, DROW, SHIP, ERROR_PUT};

struct ship;
struct map;

/*
 * @type - описывает размер и расположения корабля:
 * 1 - однопалубный
 * 2 - 2хпалубный, горизонтальный
 * 3 - 2хпалубный, вертикальный
 * 4 - 3хпалубный, горизонтальный
 * ...
 * 7 - 4хпалубный, вертикальный
 *
 * Таким образом младший бит в type описывает направление корабля
 * (горизонтальный или вертикальный, 0 и 1 соответственно).
 * Длину корабля можно получить по формуле: (length = type/2 + 1), где деление
 * целочисленное.
 *
 * @damage - описывает повреждения корабля, просто флаги, по длине самого
 * большого корабля.
 */
struct ship{
    uint8_t type;
    struct coordinates position;
    uint8_t damage[MAXSHIPSIZE]
};

/*
 * Структура описывает расположение кораблей и координаты промахов, что
 * необходимо только клиенту, сервер сможет обойтись только массивом кораблей
 */
#ifdef CLIENT_H
struct map{

    /* тут мы отслеживаем, стреляли ли по этой точке */
    char my_field[MAPSIZE][MAPSIZE];
    /* Мы не знаем, где находятся вражеские корабли и их состояние,
     * но может узнать, попали ли мы по одному из них или нет. Елси да, то
     * в соответствующую ячейку запишем SHIP */
    char en_field[MAPSIZE][MAPSIZE];
    /* наши корабли. Мы знаем где они стоят и их состояние */
    struct ship my_ships[SHIPSNUM];
    char alive;
};
#endif /* CLIENT_H */

#endif /* MAP_H */
