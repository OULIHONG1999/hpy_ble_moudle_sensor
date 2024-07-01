/*
 * @Author: OULIHONG 1756950720@qq.com
 * @Date: 2023-12-18 11:58:05
 * @LastEditors: OULIHONG 1756950720@qq.com
 * @LastEditTime: 2024-04-09 15:10:20
 * @FilePath: \蓝牙传感器驱动合集-2024年4月8日\apps\spp_and_le\examples\hpy_senser\sensor\digital_tube\digital_tube.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "digital_tube.h"
#include "asm/iic_soft.h"
#include "generic/gpio.h"
#include "sensor_cfg.h"
#include "system/includes.h"
#if DIGITAL_TUBE
#define TUBE_DATA_LEN 4
#define IIC_ID 0

// 板载led
u8 led_port = IO_PORTB_06;

struct soft_iic_config soft_iic_cfg[];
static u8 tube_data[4] = {0};
static unsigned char dig_addr[4] = {0x68, 0x6a, 0x6c, 0x6e};

char tm1650_write_data(u8 addr, u8 data)
{

    // 添加超时标志
    unsigned short error_time = 0;
    soft_iic_start(IIC_ID);
    while (!soft_iic_tx_byte(IIC_ID, addr))
    {
        clr_wdt();
        printf("send addr error %X", addr);
        error_time++;
        if (error_time > 1000)
        {
            soft_iic_stop(IIC_ID);
            return -1;
        }
    }
    error_time = 0;
    while (!soft_iic_tx_byte(IIC_ID, data))
    {
        clr_wdt();
        printf("send data error %d", data);
        error_time++;
        if (error_time > 1000)
        {
            soft_iic_stop(IIC_ID);
            return -2;
        }
    }
    soft_iic_stop(IIC_ID);
}

// 将串口得到的数据写入iic中
void uart_write_tm1650(unsigned char *data, unsigned char len)
{
    printf("get uart data!");
    put_buf(data, len);
    // 判断长度是否过短
    if (len < 5)
    {
        printf("data too short!!!\n");
        return;
    }
    char check = (char)(data[0] + data[1] + data[2] + data[3] + data[4]);
    // 检查数据准确性
    // if (check != 0)
    // {
    //     printf("data check fail!!!  %d\n", check);
    //     return;
    // }

    // printf("%d %d %d %d", data[0], data[1], data[2], data[3]);
    // 使用iic将数据显示出来
    for (u8 i = 0; i < 4; i++)
    {
        tm1650_write_data(dig_addr[i], data[i]);
    }
}

/// @brief 初始化tm1650的iic接口
void init_tm1650_iic()
{
    printf("init_tm1650 iic\n");
    soft_iic_cfg[IIC_ID].scl = IO_PORTA_07;
    soft_iic_cfg[IIC_ID].sda = IO_PORTA_08;
    soft_iic_cfg[IIC_ID].delay = 5;
    soft_iic_cfg[IIC_ID].io_pu = 1;

    soft_iic_init(IIC_ID);
}

// 将得到的数据进行更新
void updata_sensor_data(u8 **data, u8 *len)
{
    tube_data[2] = adc_get_voltage(AD_CH_VBAT);
    *len = TUBE_DATA_LEN;
    *data = tube_data;
    // put_buf(tube_data,LED_DATA_LEN);
    // memcpy(data, tube_data, LED_DATA_LEN);
}

/// @brief 处理来自主控方向的数据命令
/// @param data
/// @param len
void receive_master_data(u8 *data, u8 *len)
{
    put_buf(data, *len);
    if (data[0] == 0xab)
    {
        // 0     1     2    3    4     5
        // head  index bit1 bit2 bit3  bit4
        // 0XAB  0x00  0x00 0x00 0x00  0x00

        // printf("%d %d %d %d", data[0], data[1], data[2], data[3]);
        // 使用iic将数据显示出来
        for (u8 i = 0; i < 4; i++)
        {
            tm1650_write_data(dig_addr[i], data[i + 2]);
        }
    }
}

#define ANIMATION_MAX_LEN 5
// 未连接动画
void ble_check_timer()
{
    // 关闭并重置动画
    static u8 animation_clear = 0;
    static u8 index = 0;
    u8 ble_state = ble_gatt_server_get_work_state();
    /* u8 animation[ANIMATION_MAX_LEN][4] =
        {
            {0x01, 0x01, 0x01, 0x01},
            {0x02, 0x02, 0x02, 0x02},
            {0x04, 0x04, 0x04, 0x04},
            {0x08, 0x08, 0x08, 0x08},
            {0x10, 0x10, 0x10, 0x10},
            {0x20, 0x20, 0x20, 0x20},
        }; */
    /* u8 animation[ANIMATION_MAX_LEN][4] =
        {
            {0x01, 0x01, 0x01, 0x01},
            {0x00, 0x01, 0x01, 0x03},
            {0x00, 0x00, 0x01, 0x07},
            {0x00, 0x00, 0x00, 0x0f},
            {0x00, 0x00, 0x08, 0x0e},
            {0x00, 0x08, 0x08, 0x0c},
            {0x18, 0x08, 0x08, 0x08},
            {0x38, 0x08, 0x08, 0x00},
            {0x39, 0x00, 0x00, 0x00},
            {0x31, 0x01, 0x00, 0x00},
            {0x21, 0x01, 0x01, 0x00},
        }; */
    /* u8 animation[ANIMATION_MAX_LEN][4] =
        {
            {0x00, 0x00, 0x00, 0x00},
            {0x08, 0x00, 0x00, 0x00},
            {0x08, 0x18, 0x00, 0x00},
            {0x08, 0x18, 0x38, 0x00},
            {0x08, 0x18, 0x38, 0x39},
        }; */
    u8 animation[ANIMATION_MAX_LEN][4] =
        {
            {0x00, 0x00, 0x00, 0x00},
            {0x80, 0x00, 0x00, 0x00},
            {0x80, 0x80, 0x00, 0x00},
            {0x80, 0x80, 0x80, 0x00},
            {0x80, 0x80, 0x80, 0x80},
        };
    // 当等于2时，说明已连接
    if (ble_state == 2)
    {

        // 如果连接成功，将清除动画显示
        if (animation_clear == 0)
        {
            for (u8 i = 0; i < 4; i++)
            {
                tm1650_write_data(dig_addr[i], 0x00);
            }
        }
        animation_clear = 1;
        // 清除动画指针
        index = 0;
        return;
    }

    // 如果连接失败或断开连接，将显示动画
    animation_clear = 0;

    // 显示图案
    for (u8 i = 0; i < 4; i++)
    {
        tm1650_write_data(dig_addr[i], animation[index][i]);
    }

    if (++index >= ANIMATION_MAX_LEN)
    {
        index = 0;
    }
}

void init_sensor_device()
{
    printf("init_tm1650\n");

    u8 name[] = "HPY_DIGITAL_TUBE";
    bt_set_local_name(name, sizeof(name));

    init_tm1650_iic();
    // get_iic_adr();
    tm1650_write_data(0x48, 0x71);

    tube_data[0] = 0xab;
    tube_data[1] = 0x30;
    sys_hi_timer_add(NULL, ble_check_timer, 100);
}

#endif