//
// Created by Mars on 2023/7/8.
//

#ifndef MARS_LED_DRV_H
#define MARS_LED_DRV_H

#include <stdint.h>

typedef enum {
    BLINK_FREQ_20HZ = 25U,
    BLINK_FREQ_8HZ = 62U,
    BLINK_FREQ_2HZ = 250U,
    BLINK_FREQ_1HZ = 500U,
    BLINK_FREQ_05HZ = 1000U,
} BLINK_FREQ;

typedef struct {
    uint32_t toggle_time;
    uint32_t time;
    uint32_t toggle_num;
} BlinkConfig;

typedef struct {
    uint8_t level;
    uint8_t set_level;
    uint8_t max_level;
    uint8_t min_level;
    uint32_t step;
    uint32_t set_time;  // ms
    uint32_t time;      // ms
} BreathConfig;

struct Led_t {
    uint8_t id;
    BlinkConfig blink;
    BreathConfig breath;
    void (*led_on)(void);
    void (*led_off)(void);
    void (*led_toggle)(void);
    void (*led_level_set)(uint8_t level);
    struct Led_t *next;
};

typedef struct Led_t LedHandle;
typedef struct Led_t *pLedHandle;

void mars_led_init(void (*init)(void));
void mars_led_reset(void);
void mars_led_register(pLedHandle pLed);
void mars_led_run(void);
void mars_led_blink_start(uint8_t id, BLINK_FREQ freq, uint32_t time);
void mars_led_blink_stop(uint8_t id);
void mars_led_breath_start(uint8_t id, uint8_t level, uint32_t time);
void mars_led_breath_stop(uint8_t id);

#endif //MARS_LED_DRV_H
