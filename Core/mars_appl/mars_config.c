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
static void led_yellow_on(void) {
    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
}

static void led_yellow_off(void) {
    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
}

static void led_yellow_toggle(void) {
    HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
}

static void led_blue_on(void) {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}

static void led_blue_off(void) {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

static void led_blue_toggle(void) {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

static void led_red_on(void) {
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
}

static void led_red_off(void) {
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}

static void led_red_toggle(void) {
    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
}

static LedHandle led_yellow = {
        .led_on = led_yellow_on,
        .led_off = led_yellow_off,
        .led_toggle = led_yellow_toggle,
};

static LedHandle led_blue = {
        .led_on = led_blue_on,
        .led_off = led_blue_off,
        .led_toggle = led_blue_toggle,
};

static LedHandle led_red = {
        .led_on = led_red_on,
        .led_off = led_red_off,
        .led_toggle = led_red_toggle,
};

void led_init(void) {
    mars_led_register(&led_yellow);
    mars_led_register(&led_blue);
    mars_led_register(&led_red);
}

/* led config end */

/* key config start */
#define KEY_VALUE_1     1
static void key1_single_event(void) {
    HAL_UART_Transmit(&huart3, (uint8_t *)"key1_single_event()\n", 20, 0xFFFF);
}

static void key1_double_event(void) {
    HAL_UART_Transmit(&huart3, (uint8_t *)"key1_double_event()\n", 20, 0xFFFF);
}

static void key1_long_event(void) {
    HAL_UART_Transmit(&huart3, (uint8_t *)"key1_long_event()\n", 18, 0xFFFF);
}

static void key1_up_event(void) {
    HAL_UART_Transmit(&huart3, (uint8_t *)"key1_up_event()\n", 16, 0xFFFF);
}

KeyHandle mKey = {
        .value = KEY_VALUE_1,
        .press_time = 1000,
        .ins_time = 250,
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

void task_init(void) {
    mars_key_init(key_init, key_scan);
    mars_task_register(&KeyTask);
}

/* task config end */
