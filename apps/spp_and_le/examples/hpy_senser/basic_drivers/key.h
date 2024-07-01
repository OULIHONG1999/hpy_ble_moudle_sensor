
// 定义用户交互事件枚举类型
typedef enum
{
    CLICK,           // 单击事件
    LONG_PRESS,      // 长按事件
    LONG_PRESS_HOLD, // 长按时持续状态
    LONG_PRESS_UP,   // 长按结束事件（手指抬起）
    DOUBLE_CLICK,    // 双击事件
    TRIPLE_CLICK,    // 三连击事件
    FOURTH_CLICK,    // 四连击事件
    FIRTH_CLICK,     // 五连击事件

} UserInteractionEvent; // 命名该枚举类型，方便后续引用和理解

typedef struct
{
    unsigned char index;
    unsigned char key_event;
    void (*callback)(void); // 回调函数
} key_callback_t;

void key_event_callback(unsigned char key_event);
void key_register_callback(unsigned char key_event, void (*callback)(void));
void key_delete_callback(unsigned char key_event);
#pragma once
