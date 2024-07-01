/*
 * @Author: OULIHONG 1756950720@qq.com
 * @Date: 2024-04-08 14:43:34
 * @LastEditors: OULIHONG 1756950720@qq.com
 * @LastEditTime: 2024-04-08 17:10:15
 * @FilePath: \蓝牙传感器驱动合集-2024年4月8日\apps\spp_and_le\examples\hpy_senser\sensor\track_following\track_following.c
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "sensor_cfg.h"
#if TRACK_FOLLOWING

#include "track_following.h"
#include "system/includes.h"
#include "asm/gpio.h"
#include "asm/adc_api.h"

#define ADC_PORT IO_PORTA_03
#define ADC_CHANNEL AD_CH_PA3
#define DATA_LEN 5
static u8 tracking_data[DATA_LEN] = {0};

// 初始化传感器
void init_sensor_device()
{
    u8 name[] = {'H', 'P', 'Y', '_', 'B', 'L', 'E', '_', 'T', 'R', 'C', '\0'};
    bt_set_local_name(name, sizeof(name));

    // 调用adc初始化函数
    adc_add_sample_ch(ADC_CHANNEL);
    gpio_set_direction(ADC_PORT, 1);
    gpio_set_pull_down(ADC_PORT, 0);
    gpio_set_pull_up(ADC_PORT, 0);
    // gpio_set_die(ADC_PORT, 0);

    // 初始化数据包头
    tracking_data[0] = 0xab;
    tracking_data[1] = 0x2f;
    printf("init_track_following_sensor_device\n");
}

// 更新最新数据到数组中，蓝牙模块定时调用此函数
void updata_sensor_data(u8 **data, u8 *len)
{
    u32 vbat = adc_get_voltage(AD_CH_VBAT);
    printf("x=0,vbat=%d\n", vbat);
    // 更新电量信息
    tracking_data[2] = adc_get_voltage(AD_CH_VBAT);
    printf("x=0,adc=%d\n", adc_get_value(ADC_CHANNEL));
    // 更新ADC值
    tracking_data[3] = adc_get_value(ADC_CHANNEL) * 255 / 1024; // 0~1024

    // 更新数据长度
    *len = DATA_LEN;
    // 更新数据
    *data = tracking_data;
}

void receive_master_data(u8 *data, u8 *len)
{
    put_buf(data, *len);
}
#endif