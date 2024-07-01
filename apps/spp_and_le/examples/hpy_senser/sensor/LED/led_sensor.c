/*
 * @Author: OULIHONG 1756950720@qq.com
 * @Date: 2024-04-08 09:55:03
 * @LastEditors: OULIHONG 1756950720@qq.com
 * @LastEditTime: 2024-04-09 15:23:58
 * @FilePath: \蓝牙传感器驱动合集-2024年4月8日\apps\spp_and_le\examples\hpy_senser\sensor\LED\led_sensor.c
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "system/includes.h"
#include "sensor_cfg.h"
#include "led_sensor.h"

#if SINGLE_LED

#define LED_PORT IO_PORT_DM
#define LED_DATA_LEN 4
u8 led_port = IO_PORTB_00;
static u8 led_data[LED_DATA_LEN] = {0};
static u8 led_state = 0;

void init_sensor_device()
{
    u8 name[] = {'H', 'P', 'Y', '_', 'B', 'L', 'E', '_', 'L', 'E', 'D', '\0'};
    bt_set_local_name(name, sizeof(name));
    gpio_set_die(LED_PORT, 1);
    gpio_set_pull_up(LED_PORT, 0);
    gpio_set_pull_down(LED_PORT, 1);
    gpio_set_direction(LED_PORT, 0);
    gpio_write(LED_PORT, 0);

    led_data[0] = 0xab;
    led_data[1] = 0x28;
    led_data[2] = led_state;
}

static void set_led_state(u8 value)
{
    gpio_write(LED_PORT, value);
}

// 将得到的数据进行更新
void updata_sensor_data(u8 **data, u8 *len)
{
    led_data[2] = adc_get_voltage(AD_CH_VBAT);
    led_data[3] = led_state;
    *len = LED_DATA_LEN;
    *data = led_data;
    // put_buf(led_data,LED_DATA_LEN);
    // memcpy(data, led_data, LED_DATA_LEN);
}

/// @brief 处理来自主控方向的数据命令
/// @param data
/// @param len
void receive_master_data(u8 *data, u8 *len)
{
    // put_buf(data, *len);
    if (data[0] == 0xab)
    {
        // 0      1      2
        // head   index  command
        // 0XAB   0xff   0x00

        printf("get command %d", data[3]);
        led_state = data[2];
        set_led_state(led_state);
    }
}

#endif