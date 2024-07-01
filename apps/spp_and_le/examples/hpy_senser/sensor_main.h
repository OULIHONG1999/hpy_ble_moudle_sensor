/*
 * @Author: OULIHONG 1756950720@qq.com
 * @Date: 2024-03-29 14:45:39
 * @LastEditors: OULIHONG 1756950720@qq.com
 * @LastEditTime: 2024-03-29 14:45:43
 * @FilePath: \蓝牙传感器驱动合集-2024年3月27日\apps\spp_and_le\examples\hpy_senser\sensor_main.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */

#pragma once
void ble_data_callback(unsigned char *data, unsigned char len);

void sensor_main(void *p);
