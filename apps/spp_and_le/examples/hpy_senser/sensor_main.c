/*
 * @Author: OULIHONG 1756950720@qq.com
 * @Date: 2024-04-08 09:55:03
 * @LastEditors: OULIHONG 1756950720@qq.com
 * @LastEditTime: 2024-04-09 11:37:33
 * @FilePath: \蓝牙传感器驱动合集-2024年4月8日\apps\spp_and_le\examples\hpy_senser\sensor_main.c
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
/*
    功能实现流程整理
    1、初始化完成通用的基本功能，包含：按键、led、蓝牙
    2、通过结构体的方式，储存传感器需要使用的功能，并在本文件中调用
    3、不同的传感器需要实现相同的函数调用接口，包括：初始化、发送数据、接收数据
    4、不同的传感器需要在sensor_cfg.h文件中的宏定义，选择需要的传感器
    5、各个具体的功能实现通过不同的文件夹分开，通过宏定义选择是否编译

 */

#include "system/includes.h"
#include "sensor_cfg.h"
#include "ble.h"
#include "sensor_include.h"
#include "sensor_main.h"
#include "led_blink.h"
#include "key.h"

// #define led_port IO_PORTB_00
extern u8 led_port;
static u8 ble_connect_state = 0;

typedef struct
{
    u8 *sensor_data;                   // 数据缓存区
    u8 sensor_data_len;                // 数据长度
    void (*recv)(u8 *data, u8 *len);   // 收到数据的回调函数
    void (*updata)(u8 *data, u8 *len); // 发送数据的回调函数
    void (*init_sensor)();             // 初始化传感器的回调函数
} Sensor_s;

// 初始化Sensor_s结构体
static Sensor_s sensor = {
    .sensor_data = NULL,
    .sensor_data_len = 0,
    .updata = updata_sensor_data,
    .recv = receive_master_data,
    .init_sensor = init_sensor_device,
};

void send_data_timer()
{
    sensor.updata(&sensor.sensor_data, &sensor.sensor_data_len);
    if (ble_connect_state == 2)
    {
        ble_send_data(sensor.sensor_data, sensor.sensor_data_len);
    }

    put_buf(sensor.sensor_data, sensor.sensor_data_len);
}

void ble_data_callback(u8 *data, u8 len)
{
    sensor.recv(data, &len);
}

void ble_work_state_check_timer()
{
    ble_connect_state = ble_gatt_server_get_work_state();
    // printf("ble_work_state_check_timer: %d\n", state);
    if (ble_connect_state == 2)
        set_led_state(led_port, LED_KEEP_ON);
    else
        set_led_state(led_port, LED_BLINK);
}

// 三击断开连接
void disconnect_master_callback()
{
    printf("disconnect device\n");
    // 断开连接
    trans_disconnect();
}

// 长按关机
void close_device_callback()
{
    printf("关机。。。\n");
    // 关机
    extern void spple_set_soft_poweroff(void);
    spple_set_soft_poweroff();
}

void init_key()
{
    key_register_callback(TRIPLE_CLICK, disconnect_master_callback);
    key_register_callback(LONG_PRESS, close_device_callback);
}

void init_led()
{
    add_led(led_port, LED_BLINK_SLOW, LED_BLINK);
    sys_hi_timer_add(NULL, led_blink_timer, 10);
}

void init_sensor()
{
    sensor.init_sensor();
}


/*
是否需要有优先级标志

 */
void sensor_main(void *p)
{
    init_led();
    init_key();
    init_sensor();
    sys_hi_timer_add(NULL, ble_work_state_check_timer, 100);
    sys_hi_timer_add(NULL, send_data_timer, 100);

    while (1)
    {
        clr_wdt();
        os_time_dly(1);
    }
}