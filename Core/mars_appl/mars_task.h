//
// Created by Mars on 2023/7/17.
//

#ifndef MARS_TASK_H
#define MARS_TASK_H

#include <stdbool.h>
#include <stdint.h>

struct TASK_t {
    uint32_t time;
    uint32_t time_old;
    void (*task)(void);
    struct TASK_t *next;
};

typedef struct TASK_t TaskHandle;
typedef struct TASK_t *pTaskHandle;

void mars_task_init(void (*init)(void));
void mars_task_register(pTaskHandle pTask);
void mars_task_time_set(uint8_t id, uint32_t time);
void mars_task(void);

#endif //MARS_TASK_H
