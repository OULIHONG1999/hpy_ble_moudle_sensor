#pragma once

void init_sensor_device();

void updata_sensor_data(u8 **data, u8 *len);

void receive_master_data(u8 *data, u8 *len);
