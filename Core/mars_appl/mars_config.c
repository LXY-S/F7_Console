//
// Created by Mars on 2023/7/16.
//

#include "mars_config.h"
#include "gpio.h"
#include "usart.h"

/* shell config start*/
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

/* led config start*/
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

LedHandle led_yellow = {
        .led_on = led_yellow_on,
        .led_off = led_yellow_off,
        .led_toggle = led_yellow_toggle,
};

LedHandle led_blue = {
        .led_on = led_blue_on,
        .led_off = led_blue_off,
        .led_toggle = led_blue_toggle,
};

LedHandle led_red = {
        .led_on = led_red_on,
        .led_off = led_red_off,
        .led_toggle = led_red_toggle,
};

void led_init(void) {
    mars_led_register(&led_yellow);
    mars_led_register(&led_blue);
    mars_led_register(&led_red);
//    mars_led_blink_start(1, BLINK_FREQ_1HZ, 30000);
//    mars_led_blink_start(2, BLINK_FREQ_2HZ, 30000);
//    mars_led_blink_start(3, BLINK_FREQ_8HZ, 30000);
}

/* led config end */
