//
// Created by Mars on 2023/7/8.
//

#include <stdlib.h>
#include "mars_led_drv.h"

static pLedHandle mLeds = NULL;
static uint8_t mLedNum = 0;

extern uint32_t time_get(void);

static void led_blink(pLedHandle pLed) {
    if (pLed->blink.toggle_num && pLed->blink.toggle_time + pLed->blink.time <= time_get()) {
        if (pLed->led_toggle)
            pLed->led_toggle();
        (pLed->blink.toggle_num)--;
        pLed->blink.time = time_get();
    }
}

static void led_breath(pLedHandle pLed) {
    if (!pLed->breath.set_time)
        return;

    if (pLed->breath.time + pLed->breath.time_old <= time_get()) {
        pLed->breath.time_old = time_get();
        if (pLed->breath.set_level > pLed->breath.level) {
            pLed->breath.level++;
        } else if (pLed->breath.set_level < pLed->breath.level) {
            pLed->breath.level--;
        } else {
            pLed->breath.set_time = 0;
        }
        if (pLed->led_level_set) {
            if (pLed->blink.toggle_num) {
                if (pLed->blink.toggle_num % 2)
                    pLed->led_level_set(pLed->breath.level);
            } else {
                pLed->led_level_set(pLed->breath.level);
            }
        }
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

void mars_led_run(void) {
    if (mLeds == NULL)
        return;

    pLedHandle tmp = mLeds;
    while (tmp) {
        led_blink(tmp);
        led_breath(tmp);
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

    uint8_t flag = 0;
    if (tmp->blink.toggle_num % 2)
        flag = 1;

    tmp->blink.toggle_time = freq;
    tmp->blink.toggle_num = time / freq;
    if (tmp->blink.toggle_num % 2 || flag)
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
    tmp->breath.time_old = time_get();
    tmp->breath.time = time / (abs(tmp->breath.set_level - tmp->breath.level));
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
}

void mars_led_switch(uint8_t id, bool onoff) {
    if (id == 0 || id > mLedNum)
        return;

    pLedHandle tmp = mLeds;
    while (tmp->id != id) {
        tmp = tmp->next;
    }

    if (tmp->led_level_set)
        tmp->led_level_set(onoff ? tmp->breath.level : 0);
}

void mars_leds_switch(bool onoff) {
    pLedHandle tmp = mLeds;
    while (tmp) {
        if (tmp->led_level_set)
            tmp->led_level_set(onoff ? tmp->breath.level : 0);
        tmp = tmp->next;
    }
}
