//
// Created by Mars on 2023/7/8.
//

#include <stddef.h>
#include "mars_key_drv.h"

static pKeyHandle mKeys = NULL;

static uint8_t (*key_scan)(void) = NULL;

extern uint32_t time_get(void);

void mars_key_init(void (*init)(void), uint8_t (*scan)(void)) {
    if (init)
        init();
    if (scan)
        key_scan = scan;
}

void mars_key_reset(void) {
    mKeys = NULL;
}

void mars_key_register(pKeyHandle pKey) {
    if (pKey == NULL)
        return;

    if (mKeys == NULL) {
        mKeys = pKey;
        return;
    }

    pKeyHandle tmp = mKeys;
    while (tmp->next) {
        tmp = tmp->next;
    }
    tmp->next = pKey;
}

void mars_key_proc(void) {
    static uint8_t Key_Old = 0;
    static uint32_t Key_Time_Down_First = 0;  //记录按键 第一次 按下时的时刻
//    static uint32_t Key_Time_Down_Second = 0; //记录按键 第二次 按下时的时刻
    static uint8_t Key_First = 0;   //记录按键 第一次 按键值
    static uint8_t Key_Second = 0;  //记录按键 第二次 按键值
    static pKeyHandle key_Record = NULL;
    static void (*long_up_event)(void);

    if (key_scan == NULL)
        return;

    uint8_t Key_Temp, Key_Down, Key_Up;

    Key_Temp = key_scan();
    Key_Down = Key_Temp & (Key_Old ^ Key_Temp);   //按下为按键值，其它为0
    Key_Up = ~Key_Temp & (Key_Old ^ Key_Temp);  //松手为抬起前的按键值，其他为0
    Key_Old = Key_Temp;


    if (Key_Down) {
        pKeyHandle tmp = mKeys;
        while (tmp) {
            if (Key_Down == tmp->value) {
                if ((Key_First == 0) && (Key_Second == 0)) { //未有按键按下
                    Key_Time_Down_First = time_get();  //记录按键 第一次 按下时的时刻
                    Key_First = Key_Down;  //记录按键 第一次 按键值
                    key_Record = tmp;
                } else if ((Key_First != 0) && (Key_Second == 0)) { //已有一次按键按下，此次是第二次
//                    Key_Time_Down_Second = time_get();  //记录按键 第二次 按下时的时刻
                    Key_Second = Key_Down;  //记录按键 第二次 按键值
                    key_Record = tmp;
                }
                break;
            }
            tmp = tmp->next;
        }
    }

    static uint8_t longFlag = 1;
    if (key_Record) {
        if (Key_Temp == 0)
            longFlag = 0;
        if ((Key_First != 0) && !longFlag &&
            (time_get() - Key_Time_Down_First > key_Record->ins_time)) { //有第一次按键按下，并且时间 > ins_time
            if (Key_Second == 0) {   //第二次没有按下， 单击
                if (key_Record->single_event)
                    key_Record->single_event();
            } else if (Key_Second == Key_First) {   //第二次有按下，且和第一次相等 ，双击
                if (key_Record->double_event)
                    key_Record->double_event();
            }
            key_Record = NULL;  //清零
            Key_First = 0;
            Key_Second = 0;
            longFlag = 1;
        }

        if ((Key_First != 0) && (Key_Second == 0) &&
            (time_get() - Key_Time_Down_First > key_Record->press_time)) { //长按按下
            if (key_Record->long_event)
                key_Record->long_event();
            if (key_Record->up_event)
                long_up_event = key_Record->up_event;
            key_Record = NULL;  //清零
            Key_First = 0;
        }
    }

    if (Key_Up && long_up_event) {  //长按抬起
        long_up_event();
        longFlag = 1;
        long_up_event = NULL;
    }
}
