//
// Created by Mars on 2023/7/17.
//

#include <stddef.h>
#include "mars_task.h"

static pTaskHandle mTask;

extern uint32_t time_get(void);

void mars_task_init(void (*init)(void)) {
    init();
}

void mars_task_register(pTaskHandle pTask) {
    if (pTask == NULL) {
        return;
    }

    if (mTask == NULL) {
        mTask = pTask;
        return;
    }

    pTaskHandle tmp = mTask;
    while (tmp->next) {
        tmp = tmp->next;
    }
    tmp->next = pTask;
    pTask->time_old = time_get();
}

void mars_task_time_set(uint8_t id, uint32_t time) {
    pTaskHandle tmp = mTask;
    uint8_t i = 0;
    while (tmp) {
        if (id == i++) {
            tmp->time = time;
            break;
        } else {
            tmp = tmp->next;
        }
    }
}

void mars_task(void) {
    pTaskHandle tmp = mTask;

    while (tmp) {
        if (tmp->time_old + tmp->time <= time_get()) {
            tmp->time_old = time_get();
            if (tmp->task) {
                tmp->task();
            }
        }
        tmp = tmp->next;
    }
}
