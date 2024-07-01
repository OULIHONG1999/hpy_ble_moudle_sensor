/*
 * @Author: OULIHONG 1756950720@qq.com
 * @Date: 2024-04-08 11:05:08
 * @LastEditors: OULIHONG 1756950720@qq.com
 * @LastEditTime: 2024-04-08 14:48:40
 * @FilePath: \蓝牙传感器驱动合集-2024年4月8日\apps\spp_and_le\examples\hpy_senser\sensor\button\button.c
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "sensor_cfg.h"
#if BUTTON

#include "button.h"
#include "system/includes.h"
#include "asm/gpio.h"
#include "asm/adc_api.h"

#define BUTTON_PORT IO_PORTA_03
#define DATA_LEN 5
static u8 button_data[DATA_LEN] = {0};
u8 led_port = IO_PORTB_06;
// 初始化传感器
void init_sensor_device()
{
    gpio_set_direction(BUTTON_PORT, 1);
    gpio_set_pull_down(BUTTON_PORT, 0);
    gpio_set_pull_up(BUTTON_PORT, 1);
    gpio_set_die(BUTTON_PORT, 1);

    // 初始化数据包头
    button_data[0] = 0xab;
    button_data[1] = 0x2a;
    printf("init button sensor device\n");
}

// 更新最新数据到数组中，蓝牙模块定时调用此函数
void updata_sensor_data(u8 **data, u8 *len)
{
    u32 vbat = adc_get_voltage(AD_CH_VBAT);
    printf("x=0,vbat=%d\n", vbat);
    // 更新电量信息
    button_data[2] = adc_get_voltage(AD_CH_VBAT);
    // 更新按键的值
    button_data[3] = gpio_read(BUTTON_PORT);
    // 更新数据长度
    *len = DATA_LEN;
    // 更新数据
    *data = button_data;
}

void receive_master_data(u8 *data, u8 *len)
{
    put_buf(data, *len);
}
#endif