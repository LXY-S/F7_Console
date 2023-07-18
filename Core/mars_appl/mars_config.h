//
// Created by Mars on 2023/7/16.
//

#ifndef MARS_CONFIG_H
#define MARS_CONFIG_H

#include "mars_shell.h"
#include "mars_led_drv.h"
#include "mars_task.h"
#include "mars_key_drv.h"

void shell_init(void);
void shell_output(uint8_t *pData, uint16_t len);

void led_init(void);

void task_init(void);

#endif //MARS_CONFIG_H
