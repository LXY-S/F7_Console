//
// Created by Mars on 2023/7/8.
//

#ifndef MARS_KEY_DRV_H
#define MARS_KEY_DRV_H

#include "gpio.h"

struct Key_t{
    uint8_t id;
    void (*single_event)(void);
    void (*double_event)(void);
    void (*long_event)(void);
    struct Key_t* next;
};

typedef struct Key_t KeyHandle;
typedef struct Key_t *pKeyHandle;

void mars_key_init(void (*init)(void));
void mars_key_reset(void);
void mars_key_register(pKeyHandle pKey);

#endif //MARS_KEY_DRV_H
