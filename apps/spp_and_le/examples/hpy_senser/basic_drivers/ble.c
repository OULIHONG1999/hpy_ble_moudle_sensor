#include "system/includes.h"
#include "ble.h"

void ble_send_data(u8 *data, u8 len)
{
    // put_buf(data, len);
    extern void ble_trans_send_data(u8 *packet, u32 size);
    ble_trans_send_data(data, len);
}