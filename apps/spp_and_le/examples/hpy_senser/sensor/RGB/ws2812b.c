/*
 * @Author: OULIHONG 1756950720@qq.com
 * @Date: 2024-04-08 14:43:34
 * @LastEditors: OULIHONG 1756950720@qq.com
 * @LastEditTime: 2024-04-09 12:00:12
 * @FilePath: \蓝牙传感器驱动合集-2024年4月8日\apps\spp_and_le\examples\hpy_senser\sensor\RGB\ws2812b.c
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "sensor_cfg.h"
#if RGB
#include "asm/ledc.h"
#include "track_following.h"
#include "system/includes.h"
#include "asm/gpio.h"
#include "asm/adc_api.h"

#define SPI_PORT IO_PORTA_09
#define DATA_LEN 5
#define LED_NUM_MAX 1
#define led_init() ledc_init(&ws2812b_data)
#define led_send_rgbbuf(buf, num) ledc_send_rgbbuf_isr(0, buf, num, 1)
// 板载led
u8 led_port = -1;

static u8 led_rgb_val_buf[3 * LED_NUM_MAX] __attribute__((aligned(4)));
static u8 tracking_data[DATA_LEN] = {0};

const struct ledc_platform_data ws2812b_data = {
    .index = 0,
    .port = SPI_PORT,
    .idle_level = 0,
    .out_inv = 0,
    .bit_inv = 1,
    .t_unit = t_42ns,
    .t1h_cnt = 24,
    .t1l_cnt = 7,
    .t0h_cnt = 7,
    .t0l_cnt = 24,
    .t_rest_cnt = 20000,
    .cbfun = NULL,
};

void check_ble_connect_timer()
{
    u8 ble_state = ble_gatt_server_get_work_state();
    static u8 blink_state = 0;
    static u8 led_blink_on[3] __attribute__((aligned(4)))= {0xff, 0, 0};
    static u8 led_blink_off[3] __attribute__((aligned(4)))= {0, 0xff, 0};
    if (ble_state != 2)
    {
        if (blink_state == 1)
        {
            led_send_rgbbuf(led_blink_on, 1);
        }
        else
        {
            led_send_rgbbuf(led_blink_off, 1);
        }
        blink_state = !blink_state;
    }
    else
    {
        led_send_rgbbuf(led_rgb_val_buf, 1);
        blink_state = 0;
    }
}

// 初始化传感器
void init_sensor_device()
{
    u8 name[] = "HPY_WS2812B";
    bt_set_local_name(name, sizeof(name));

    led_init();

    // 初始化数据包头
    tracking_data[0] = 0xab;
    tracking_data[1] = 0x29;

    sys_hi_timer_add(NULL, check_ble_connect_timer, 500);

    printf("init HPY_WS2812B device\n");
}

// 更新最新数据到数组中，蓝牙模块定时调用此函数
void updata_sensor_data(u8 **data, u8 *len)
{
    u32 vbat = adc_get_voltage(AD_CH_VBAT);
    printf("x=0,vbat=%d\n", vbat);
    // 更新电量信息
    tracking_data[2] = adc_get_voltage(AD_CH_VBAT);
    // 更新数据长度
    *len = DATA_LEN;
    // 更新数据
    *data = tracking_data;
}

void receive_master_data(u8 *data, u8 *len)
{
    put_buf(data, *len);
    if (data[0] == 0xab)
    {
        // 0     1     2    3    4
        // head  index R    G    B
        // 0XAB  0x00  0x00 0x00 0x00
        led_rgb_val_buf[0] = data[2];
        led_rgb_val_buf[1] = data[4];
        led_rgb_val_buf[2] = data[3];
        led_send_rgbbuf(led_rgb_val_buf, 1);
    }
}
#endif