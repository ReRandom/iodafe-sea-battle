#ifndef SHOT_H
#define SHOT_H

struct coordinates {
    uint8_t x;
    uint8_t y;
};

struct shot {
    struct coordinates position;
    uint8_t status;
};

#endif /* SHOT_H */
