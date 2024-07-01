/*
 * @Author: OULIHONG 1756950720@qq.com
 * @Date: 2024-03-27 16:01:46
 * @LastEditors: OULIHONG 1756950720@qq.com
 * @LastEditTime: 2024-03-29 15:58:03
 * @FilePath: \蓝牙传感器驱动合集-2024年3月27日\apps\spp_and_le\examples\hpy_senser\basic_drivers\key.c
 * @Description:
 * 1、提供按键功能，以及按键事件的处理
 * 2、提供按键事件的注册，根据按键事件类型进行回调
 * 3、仅支持单按键
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */

#include "system/includes.h"
#include "key.h"
#include "asm/gpio.h"
#include "asm/power_interface.h"

#define MAX_CALLBACK_NUM 10
#define KEY_PORT IO_PORTA_00

// 按键注册函数数组
static key_callback_t key_callback_list[MAX_CALLBACK_NUM] = {NULL};

/// @brief 按键事件的回调函数
/// @param key_event
void key_event_callback(unsigned char key_event)
{
    // 遍历数组，找到空位
    for (int i = 0; i < MAX_CALLBACK_NUM; i++)
    {
        if (key_callback_list[i].key_event == key_event && key_callback_list[i].callback != NULL)
        {
            key_callback_list[i].callback();
        }
    }
}

/// @brief 注册按键事件回调
/// @param key_event
/// @param callback
void key_register_callback(unsigned char key_event, void (*callback)(void))
{
    // 遍历数组，找到空位
    for (int i = 0; i < MAX_CALLBACK_NUM; i++)
    {

        if (key_callback_list[i].callback == NULL)
        {
            key_callback_list[i].key_event = key_event;
            key_callback_list[i].callback = callback;
            printf("key_register_callback success %d\n", i);
            return;
        }
    }
    printf("key_register_callback fail\n");
}

// 删除已注册的回到函数
void key_delete_callback(unsigned char key_event)
{
    // 遍历数组，找到空位
    for (int i = 0; i < MAX_CALLBACK_NUM; i++)
    {
        if (key_callback_list[i].key_event == key_event)
        {
            key_callback_list[i].key_event = 0;
            key_callback_list[i].callback = NULL;
            break;
        }
    }
}
