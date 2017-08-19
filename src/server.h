#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "map.h"

#define DEFAULT_WOKERS_NUMBER 4

enum status {GETTING_MAP, GAMING, EXITING};
enum mqtypes {NEW_GAME=1};

/*
 * @status - описывает, чего мы ждём (этапы игры).
 * @current_client - игрок, чей ход сейчас идёт (исключает возможность ходить
 * дважды), описывается сокетом.
 * @buffer - буфер конкретной игры, нужен для того, чтобы не блокировать целый
 * поток из-за одного медленного клиента.
 */
struct game {
    int status;
    int sockets[2];
    struct ship ships[2][SHIPSNUM];
    int current_client;
    void *buffer;
    ssize_t buffer_size;
};

struct mqitem {
    long type;
    struct game *game;
};

struct worker_args {
    int mqid;
};

/* Сокет, через который подключаются остальные */
int connector_socket;

/*
 * Необходим для того, чтобы в случае падения потока данные об играх не
 * терялись.
 * Является двумерным массивом указателей. Размер массива верхнего уровня
 * соответствует числу потоков. Массив нижнего уровня соответствует числу игр
 * в конкретном потоке. Массив указателей реализован для упрощения
 * перераспределения памяти.
 */
struct game ** *threads_data;

/*
 * Находит структуру из массива game_list, содержащую нужный сокет, возвращает
 * указатель на искомую структуру. В случае ошибки вернёт NULL.
 */
struct game* find_game_by_socket(int socket, const struct game *game_list);

/*
 * Отправляет "выстрел" обоим игрокам, одному необходим код для того, чтобы
 * узнать, попал он или нет, другому - факт выстрела +результат.
 * В случае ошибки возвращает код ошибки.
 */
int send_shot(const struct game *game, const struct shot *shot);

/*
 * Приём выстрела от игрока, не блокирует сокет, как и поток, не делает
 * проверки на то, от того ли игрока мы ждали выстрела, заполняет структуру
 * shot.
 * В случае, если выстрел был принят не полностью, возвращает 0, иначе 1.
 * В случае ошибки возвращает код ошибки.
 */
int get_shot(int socket, struct shot *shot);

/*
 * Получение карты от конкретного игрока, заполняет структуру game. Не делает
 * проверку на валидность карты.
 * В случае, если карта не была принята полностью, возвращает 0, иначе 1.
 * В случае ошибки возвращает код ошибки.
 */
int get_map(int socket, struct game *game);

/*
 * Проверяет карту на валидность. В случае, если карта валидна возвращает 1,
 * иначе 0.
 */
int check_map(const struct ship ships[SHIPSNUM]);

/*
 * Подсчитывает число не раненых ячеек кораблей по карте игрока. Если вернёт 0,
 * значит игрок проиграл.
 */
uint8_t get_alive(const struct ship ships[SHIPSNUM]);

/*
 * Отправляет победителю и проигравшему соответствующие сообщения.
 * В случае ошибки вернёт её код.
 */
int end_game(struct game *game);

void *worker(void*);

/*
 * Не придумал названия лучше. Инициализирует данные, запускает потоки, ловит
 * сигналы и обрабатывает их, перезапускает потоки при падении, завершает их.
 * Если не удаётся перезапустить поток, его данные передаёт остальным для
 * обработки.
 */
void main_daemon();

#endif /* SERVER_H */
