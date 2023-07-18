//
// Created by Mars on 2023/7/8.
//

#include <stdlib.h>
#include "mars_led_drv.h"

static pLedHandle mLeds = NULL;
static uint8_t mLedNum = 0;

extern uint32_t time_get(void);

static void led_blink(pLedHandle pLed, uint32_t time) {
    if (pLed->blink.toggle_num && pLed->blink.toggle_time + pLed->blink.time <= time) {
        if (pLed->led_toggle)
            pLed->led_toggle();
        (pLed->blink.toggle_num)--;
        pLed->blink.time = time;
    }
}

static void led_breath(pLedHandle pLed, uint32_t time) {
    static uint32_t time_old = 0;

    if (!pLed->breath.time || !pLed->breath.set_time)
        return;

    if (pLed->breath.time + time_old >= time) {
        if (abs((int8_t)pLed->breath.set_level - (int8_t)pLed->breath.level) < pLed->breath.set_time / pLed->breath.time) {
            pLed->breath.level = pLed->breath.set_level;
        } else if (pLed->breath.set_level > pLed->breath.level) {
            pLed->breath.level += pLed->breath.set_time / pLed->breath.time;
        } else if (pLed->breath.set_level < pLed->breath.level) {
            pLed->breath.level -= pLed->breath.set_time / pLed->breath.time;
        } else {
            pLed->breath.set_time = 0;
            pLed->breath.time = 0;
        }
        if (pLed->led_level_set && pLed->blink.toggle_num % 2)
            pLed->led_level_set(pLed->breath.level);
        time_old = time;
    }
}

void mars_led_init(void (*init)(void)) {
    if (init)
        init();
}

void mars_led_reset(void) {
    mLeds = NULL;
    mLedNum = 0;
}

void mars_led_register(pLedHandle pLed) {
    if (pLed == NULL)
        return;

    if (pLed->breath.min_level > pLed->breath.max_level) {
        pLed->breath.min_level = 0;
        pLed->breath.max_level = 100;
    }
    if (pLed->breath.level > pLed->breath.max_level)
        pLed->breath.level = pLed->breath.max_level;
    if (pLed->breath.level < pLed->breath.min_level)
        pLed->breath.level = pLed->breath.min_level;

    if (mLeds == NULL) {
        mLedNum = 1;
        mLeds = pLed;
        mLeds->id = mLedNum;
        return;
    }

    pLedHandle tmp = mLeds;
    while (tmp->next) {
        tmp = tmp->next;
    }
    tmp->next = pLed;
    pLed->id = ++mLedNum;
}

void mars_led_run(uint32_t time) {
    if (mLeds == NULL)
        return;

    pLedHandle tmp = mLeds;
    while (tmp) {
        led_blink(tmp, time);
        led_breath(tmp, time);
        tmp = tmp->next;
    }
}

void mars_led_blink_start(uint8_t id, BLINK_FREQ freq, uint32_t time) {
    if (id == 0 || id > mLedNum)
        return;

    pLedHandle tmp = mLeds;
    while (tmp->id != id) {
        tmp = tmp->next;
    }

    tmp->blink.toggle_time = freq;
    tmp->blink.toggle_num = time / freq;
    if (tmp->blink.toggle_num % 2)
        tmp->blink.toggle_num++;

    tmp->blink.time = time_get();
    tmp->led_toggle();
    tmp->blink.toggle_num--;
}

void mars_led_blink_stop(uint8_t id) {
    if (id == 0 || id > mLedNum)
        return;

    pLedHandle tmp = mLeds;
    while (tmp->id != id) {
        tmp = tmp->next;
    }

    tmp->blink.toggle_num = 0;
}

void mars_led_breath_start(uint8_t id, uint8_t level, uint32_t time) {
    if (id == 0 || id > mLedNum)
        return;

    pLedHandle tmp = mLeds;
    while (tmp->id != id) {
        tmp = tmp->next;
    }

    if (level > tmp->breath.max_level)
        level = tmp->breath.max_level;
    if (level < tmp->breath.min_level)
        level = tmp->breath.min_level;

    tmp->breath.set_level = level;
    tmp->breath.set_time = time;
    tmp->breath.time = abs((int8_t)tmp->breath.set_level - (int8_t)tmp->breath.level);
}

void mars_led_breath_stop(uint8_t id) {
    if (id == 0 || id > mLedNum)
        return;

    pLedHandle tmp = mLeds;
    while (tmp->id != id) {
        tmp = tmp->next;
    }

    tmp->breath.set_level = tmp->breath.level;
    tmp->breath.set_time = 0;
    tmp->breath.time = 0;
}
