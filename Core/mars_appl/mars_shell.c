//
// Created by 19706 on 2023/7/8.
//

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <malloc.h>
#include "mars_shell.h"

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

static void shell(uint16_t argc, uint8_t *argv[]) {
    if (mShellQueue.output == NULL) {
        return;
    }
    switch (argc) {
        case 1:
            if (strcmp((char *) mStr[0], "mars") == 0) {
                mShellQueue.output((uint8_t *) "mars\ninfo\nlogo\n", 15);
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

                }
            }
            break;
        default:
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

        uint16_t num = string_split(data, len, (uint8_t *)mStr, 10);
        free(data);

        shell(num, (uint8_t **) mStr);
        memset(mStr, 0, 100);
        if (mShellQueue.output) {
            mShellQueue.output((uint8_t *) mName, 6);
        }
    }
}
