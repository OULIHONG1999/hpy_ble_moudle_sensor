/*
 * @Author: OULIHONG 1756950720@qq.com
 * @Date: 2024-03-26 11:01:55
 * @LastEditors: OULIHONG 1756950720@qq.com
 * @LastEditTime: 2024-04-08 10:41:42
 * @FilePath: \蓝牙传感器驱动合集-2024年4月8日\apps\spp_and_le\examples\hpy_senser\basic_drivers\led_blink.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */

typedef enum
{
    LED_KEEP_OFF = 1,
    LED_KEEP_ON = 2,
    LED_BLINK = 3
} LedState;

typedef enum
{
    LED_BLINK_OFF,
    LED_BLINK_SLOW = 100,
    LED_BLINK_MIDDLE = 50,
    LED_BLINK_FAST = 30,
    LED_BLINK_MAXIMUM = 10, // 可能是您想表示的“LED_BLINK_MAX”
} LedBlinkSpeed;

typedef struct
{
    unsigned int port;         // GPIO端口
    LedState state;            // LED状态（常亮、关闭、闪烁）
    u8 blink_flag;             // LED闪烁状态标志位（仅在闪烁状态有意义）
    u8 blink_cnt;              // LED闪烁计数器
    LedBlinkSpeed blink_speed; // LED闪烁速度
} LedInfo;

void add_led(unsigned int port, LedBlinkSpeed speed, LedState initial_state);
void remove_led(unsigned int port);
void set_led_state(unsigned int port, LedState state);
void led_blink_timer();
#pragma once
