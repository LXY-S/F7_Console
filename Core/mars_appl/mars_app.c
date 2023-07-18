//
// Created by Mars on 2023/7/16.
//

#include "mars_app.h"
#include "mars_config.h"
#include "tim.h"

void mars_app_init(void) {
	HAL_TIM_Base_Start_IT(&htim7);
    mars_shell_init(shell_init, shell_output);
    mars_led_init(led_init);
    mars_task_init(task_init);
}

void mars_app_run(void) {
    mars_shell_run();
    mars_led_run(time_get());
    mars_task();
}
