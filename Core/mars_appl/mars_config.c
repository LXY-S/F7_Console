//
// Created by Mars on 2023/7/16.
//

#include "mars_config.h"
#include "gpio.h"
#include "usart.h"

/* shell config start */
static uint8_t mData = 0;

void shell_init(void) {
    HAL_UART_Receive_IT(&huart3, &mData, 1);
}

void shell_output(uint8_t *pData, uint16_t len) {
    HAL_UART_Transmit(&huart3, pData, len, 0xFFFF);
}

/**
  * @brief Rx Transfer completed callbacks
  * @param huart: uart handle
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    HAL_UART_Receive_IT(&huart3, &mData, 1);
    mars_shell_input(mData);
}
/* shell config end */

/* led config start */
static uint8_t *yellow_value;
static uint8_t *blue_value;
static uint8_t *red_value;
static bool yellow_flag = false;
static bool blue_flag = false;
static bool red_flag = false;

static void led_yellow_level_set(uint8_t value) {
    if (value)
        yellow_flag = true;
    else
        yellow_flag = false;
    *((uint32_t *) 0x4000043C) = value * 10;
}

static void led_yellow_on(void) {
    led_yellow_level_set(*yellow_value);

}

static void led_yellow_off(void) {
    led_yellow_level_set(0);
}

static void led_yellow_toggle(void) {
    if (yellow_flag) {
        led_yellow_off();
        yellow_flag = false;
    } else {
        led_yellow_on();
        yellow_flag = true;
    }
}

static void led_blue_level_set(uint8_t value) {
    if (value)
        blue_flag = true;
    else
        blue_flag = false;
    *((uint32_t *) 0x40000838) = value * 10;
}

static void led_blue_on(void) {
    led_blue_level_set(*blue_value);
}

static void led_blue_off(void) {
    led_blue_level_set(0);
}

static void led_blue_toggle(void) {
    if (blue_flag) {
        led_blue_off();
        blue_flag = false;
    } else {
        led_blue_on();
        blue_flag = true;
    }
}

static void led_red_level_set(uint8_t value) {
    if (value)
        red_flag = true;
    else
        red_flag = false;
    *((uint32_t *) 0x40001834) = value * 10;
}

static void led_red_on(void) {
    led_red_level_set(*red_value);
}

static void led_red_off(void) {
    led_red_level_set(0);
}

static void led_red_toggle(void) {
    if (red_flag) {
        led_red_off();
        red_flag = false;
    } else {
        led_red_on();
        red_flag = true;
    }
}

static LedHandle led_yellow = {
        .breath.max_level = 100,
        .breath.min_level = 0,
        .breath.level = 0,
        .led_level_set = led_yellow_level_set,
        .led_on = led_yellow_on,
        .led_off = led_yellow_off,
        .led_toggle = led_yellow_toggle,
};

static LedHandle led_blue = {
        .breath.max_level = 100,
        .breath.min_level = 0,
        .breath.level = 0,
        .led_level_set = led_blue_level_set,
        .led_on = led_blue_on,
        .led_off = led_blue_off,
        .led_toggle = led_blue_toggle,
};

static LedHandle led_red = {
        .breath.max_level = 100,
        .breath.min_level = 0,
        .breath.level = 0,
        .led_level_set = led_red_level_set,
        .led_on = led_red_on,
        .led_off = led_red_off,
        .led_toggle = led_red_toggle,
};

void led_init(void) {
    yellow_value = &led_yellow.breath.level;
    blue_value = &led_blue.breath.level;
    red_value = &led_red.breath.level;
    mars_led_register(&led_yellow);
    mars_led_register(&led_blue);
    mars_led_register(&led_red);

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
}

/* led config end */

/* key config start */
#define KEY_VALUE_1     1

static void key1_single_event(void) {
    HAL_UART_Transmit(&huart3, (uint8_t *) "key1_single_event()\n", 20, 0xFFFF);
}

static void key1_double_event(void) {
    HAL_UART_Transmit(&huart3, (uint8_t *) "key1_double_event()\n", 20, 0xFFFF);
}

static void key1_long_event(void) {
    HAL_UART_Transmit(&huart3, (uint8_t *) "key1_long_event()\n", 18, 0xFFFF);
}

static void key1_up_event(void) {
    HAL_UART_Transmit(&huart3, (uint8_t *) "key1_up_event()\n", 16, 0xFFFF);
}

KeyHandle mKey = {
        .value = KEY_VALUE_1,
        .press_time = 2000,
        .ins_time = 300,
        .single_event = key1_single_event,
        .double_event = key1_double_event,
        .long_event = key1_long_event,
        .up_event = key1_up_event
};

static void key_init(void) {
    mars_key_register(&mKey);
}

static uint8_t key_scan(void) {
    uint8_t value = 0;

    if (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_SET)
        value = KEY_VALUE_1;

    return value;
}

/* key config end */

/* task config start */
static TaskHandle KeyTask = {
        .time = 10,
        .task = mars_key_proc
};

static TaskHandle ledTask = {
        .time = 1,
        .task = mars_led_run,
};

void task_init(void) {
    mars_key_init(key_init, key_scan);
    mars_task_register(&KeyTask);

    mars_led_init(led_init);
    mars_task_register(&ledTask);
}

/* task config end */
