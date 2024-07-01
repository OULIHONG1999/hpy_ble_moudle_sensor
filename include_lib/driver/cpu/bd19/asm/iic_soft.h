/*
 * @Author: OULIHONG 1756950720@qq.com
 * @Date: 2024-04-08 09:55:04
 * @LastEditors: OULIHONG 1756950720@qq.com
 * @LastEditTime: 2024-04-09 10:12:44
 * @FilePath: \蓝牙传感器驱动合集-2024年4月8日\include_lib\driver\cpu\bd19\asm\iic_soft.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef	_IIC_SOFT_H_
#define _IIC_SOFT_H_

#include "generic/typedef.h"

typedef const int soft_iic_dev;

struct soft_iic_config {
    int scl;
    int sda;
    u32 delay;
    u8 io_pu;
};

extern struct soft_iic_config soft_iic_cfg[];

int soft_iic_init(soft_iic_dev iic);
void soft_iic_uninit(soft_iic_dev iic);
void soft_iic_suspend(soft_iic_dev iic);
void soft_iic_resume(soft_iic_dev iic);
void soft_iic_start(soft_iic_dev iic);
void soft_iic_stop(soft_iic_dev iic);
u8 soft_iic_tx_byte(soft_iic_dev iic, u8 byte);
u8 soft_iic_rx_byte(soft_iic_dev iic, u8 ack);
int soft_iic_read_buf(soft_iic_dev iic, void *buf, int len);
int soft_iic_write_buf(soft_iic_dev iic, const void *buf, int len);
#endif

