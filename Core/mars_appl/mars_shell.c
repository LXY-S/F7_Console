//
// Created by Mars on 2023/7/8.
//

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <malloc.h>
#include <stdlib.h>
#include "mars_shell.h"
#include "mars_led_drv.h"

#define SHELL_DATA_LEN 1024

typedef struct {
    bool flag;
    uint16_t head;
    uint8_t data[SHELL_DATA_LEN];

    void (*output)(uint8_t *data, uint16_t len);
} QueueHandle, *pQueueHandle;

static QueueHandle mShellQueue;
static const uint8_t mName[7] = "Mars >";
static uint8_t mStr[10][10] = {0};

static uint16_t string_split(uint8_t *string, uint16_t len, uint8_t *argv, uint16_t size) {
    uint16_t argc = 0;
    uint8_t *ch = malloc(len);
    strncpy((char *) ch, (char *) string, (size_t) len);
    uint8_t *str = (char *) strtok((char *) ch, " ");
    while (str != NULL) {
        strncpy(argv + (argc * size), str, strlen(str));
        argc++;
        str = strtok(NULL, " ");
        if (str == NULL)
            break;
    }
    free(ch);
    return argc;
}

static void led_shell(void) {
    uint8_t led_id = mStr[1][3];
    BLINK_FREQ freq;

    if (strcmp((char *) mStr[3], "0.5Hz") == 0) {
        freq = BLINK_FREQ_05HZ;
    } else if (strcmp((char *) mStr[3], "1Hz") == 0) {
        freq = BLINK_FREQ_1HZ;
    } else if (strcmp((char *) mStr[3], "2Hz") == 0) {
        freq = BLINK_FREQ_2HZ;
    } else if (strcmp((char *) mStr[3], "8Hz") == 0) {
        freq = BLINK_FREQ_8HZ;
    } else if (strcmp((char *) mStr[3], "20Hz") == 0) {
        freq = BLINK_FREQ_20HZ;
    } else {
        mShellQueue.output((uint8_t *) "eg: mars led1 blink 8Hz 1000\n", 29);
        return;
    }

    switch (led_id) {
        case '1':
            if (strcmp((char *) mStr[2], "blink") == 0) {
                uint32_t time = (int) atoi((char *) mStr[4]);
                mars_led_blink_start(1, freq, time);
            } else {
                mShellQueue.output((uint8_t *) "eg: mars led1 blink 8Hz 1000\n", 29);
            }
            break;
        case '2':
            if (strcmp((char *) mStr[2], "blink") == 0) {
                uint32_t time = (int) atoi((char *) mStr[4]);
                mars_led_blink_start(2, freq, time);
            } else {
                mShellQueue.output((uint8_t *) "eg: mars led1 blink 8Hz 1000\n", 29);
            }
            break;
        case '3':
            if (strcmp((char *) mStr[2], "blink") == 0) {
                uint32_t time = (int) atoi((char *) mStr[4]);
                mars_led_blink_start(3, freq, time);
            } else {
                mShellQueue.output((uint8_t *) "eg: mars led1 blink 8Hz 1000\n", 29);
            }
            break;
        default:
            mShellQueue.output((uint8_t *) "eg: mars led1 blink 8Hz 1000\n", 29);
            break;
    }
}

static void shell(uint16_t argc, uint8_t *argv[]) {
    if (mShellQueue.output == NULL) {
        return;
    }
    switch (argc) {
        case 1:
            if (strcmp((char *) mStr[0], "mars") == 0) {
                mShellQueue.output((uint8_t *) "mars\ninfo\nlogo\nled\n", 19);
            } else {

            }
            break;
        case 2:
            if ((strcmp((char *) mStr[0], "mars") == 0)) {
                if (strcmp((char *) mStr[1], "info") == 0) {
                    mShellQueue.output((uint8_t *) "Nucleo-F767ZI\n", 14);
                } else if (strcmp((char *) mStr[1], "logo") == 0) {
                    mShellQueue.output((uint8_t *) "    _/      _/                                \n", 47);
                    mShellQueue.output((uint8_t *) "   _/_/  _/_/    _/_/_/  _/  _/_/    _/_/_/   \n", 47);
                    mShellQueue.output((uint8_t *) "  _/  _/  _/  _/    _/  _/_/      _/_/        \n", 47);
                    mShellQueue.output((uint8_t *) " _/      _/  _/    _/  _/            _/_/     \n", 47);
                    mShellQueue.output((uint8_t *) "_/      _/    _/_/_/  _/        _/_/_/        \n", 47);
                } else {
                    mShellQueue.output((uint8_t *) "mars\ninfo\nlogo\nled\n", 19);
                }
            }
            break;
        case 5:
            if ((strcmp((char *) mStr[0], "mars") == 0)) {
                if (strncmp((char *) mStr[1], "led", 3) == 0) {
                    led_shell();
                } else {
                    mShellQueue.output((uint8_t *) "mars\ninfo\nlogo\nled\n", 19);
                }
            }
        default:
            mShellQueue.output((uint8_t *) "mars\ninfo\nlogo\nled\n", 19);
            break;
    }
}

void mars_shell_init(void (*init)(void), void (*output)(uint8_t *data, uint16_t len)) {
    init();
    memset(mShellQueue.data, 0, SHELL_DATA_LEN);
    mShellQueue.head = 0;
    mShellQueue.output = output;
}

void mars_shell_input(unsigned char data) {
    if (mShellQueue.head == SHELL_DATA_LEN - 1) {
        mShellQueue.flag = 1;
        mShellQueue.data[mShellQueue.head] = '\n';
        return;
    }
    if (data == '\n') {
        mShellQueue.flag = 1;
    }
    mShellQueue.data[mShellQueue.head++] = data;
}

void mars_shell_run(void) {
    if (mShellQueue.flag) {
        uint8_t *data = malloc(mShellQueue.head - 1);
        strncpy((char *) data, (char *) mShellQueue.data, mShellQueue.head - 1);
        data[mShellQueue.head - 2] = 0;
        uint16_t len = mShellQueue.head;

        memset(mShellQueue.data, 0, SHELL_DATA_LEN);
        mShellQueue.head = 0;
        mShellQueue.flag = 0;

        uint16_t num = string_split(data, len, (uint8_t *) mStr, 10);
        free(data);

        shell(num, (uint8_t **) mStr);
        memset(mStr, 0, 100);
        if (mShellQueue.output) {
            mShellQueue.output((uint8_t *) mName, 6);
        }
    }
}
