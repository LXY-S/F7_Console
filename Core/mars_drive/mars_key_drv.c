//
// Created by Mars on 2023/7/8.
//

#include "mars_key_drv.h"

static pKeyHandle mKeys = NULL;
static uint8_t mKeyNum = 0;

static uint8_t key_scan(void) {
    uint8_t key_value = 0;

    return key_value;
}

void mars_key_init(void (*init)(void)) {
    if (init)
        init();
}

void mars_key_reset(void) {
    mKeys = NULL;
    mKeyNum = 0;
}

void mars_key_register(pKeyHandle pKey) {
    if (pKey == NULL)
        return;

    pKeyHandle tmp = mKeys;
    while (tmp) {
        tmp = tmp->next;
    }
    mKeyNum++;
    tmp = pKey;
    tmp->id = mKeyNum;
}
