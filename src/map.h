/* map.h - тут хранится информация о карте */

#ifndef MAP_H_
#define MAP_H
/* TODO: Можно снизить лишний расход памяти тем, что мы не храним положение
 * корабля в двумерном массиве. */

enum message {NOTHING, HIT, DROW};

struct ship{
    /* максимальная размер корабля - 4 клетки
     * мы можем заполнить весь 1байтовый char нулями. при создании корабля 
     * определенное количество бит заполняется единицами. Например 
     * 00001111 будет означать "четырехпалубный" корабль. При попадании это
     * количество бит будет уменьшаться на 1(делаем сдвиг вправо). Когда все 
     * биты станут нулями, то корабль "потоплен" */
    char length;

    /* поле 10x10, где каждый элемент представляет из себя клетку поля. 
     * В каждом элементе массива мы храним:
     * NOTHING - клетка пустая
     * SHIP - на этом месте стоит свой корабль
     * При создании корабля, соотвествующие биты заполняются
     * Если в по кораблю попали, то соотвествующий бит становится NOTHING */
    char position[10][10];
};


struct map{

    /* тут мы отслеживаем, стреляли ли по этой точке */
    char my_field[10][10];
    /* Мы не знаем, где находятся вражеские корабли и их состояние,
     * но может узнать, попали ли мы по одному из них или нет. Елси да, то
     * в соотвествующую ячейку запищем SHIP */
    char en_field[10][10];
    /* наши корабли. Мы знаем где они стоят и их состояние */
    struct ship my_ships[10];
    char alive;
};

#endif
