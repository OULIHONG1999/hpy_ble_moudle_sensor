#pragma once

void init_tm1650_iic();

void uart_write_tm1650(unsigned char *data, unsigned char len);
