#include "asm/gpio.h"
#include "led_blink.h"

#define MAX_LED_NUM 10 // 最大LED数量

static LedInfo leds[MAX_LED_NUM]; // LED信息数组
static int led_count = 0;         // 当前LED数量

// 初始化LED结构体数组中的元素
static void init_led(LedInfo *led, unsigned int port, LedBlinkSpeed speed, LedState initial_state)
{
    led->port = port;
    led->state = initial_state;
    led->blink_flag = initial_state == LED_KEEP_ON ? 1 : 0;
    led->blink_speed = speed;

    gpio_set_die(led->port, 1);
    gpio_set_pull_up(led->port, 0);
    gpio_set_pull_down(led->port, 1);
    gpio_set_direction(led->port, 0);
    gpio_write(led->port, led->state == LED_KEEP_ON ? 1 : 0);

    printf("add_led port:%d, speed:%d, initial_state:%d\n", led->port, speed, led->blink_flag);
}
// 添加LED
void add_led(unsigned int port, LedBlinkSpeed speed, LedState initial_state)
{

    if (led_count < MAX_LED_NUM)
    {
        init_led(&leds[led_count++], port, speed, initial_state);
    }
    else
    {
        printf("Error: Maximum number of LEDs reached.\n");
    }
}

/**
 * 删除指定端口的LED
 *
 * 本函数通过给定的端口标识，从一个全局LED数组中找到对应的LED，并将其从数组中删除。
 * 注意：此函数假设LED数组是按端口编号顺序排列的。
 *
 * @param port 要删除的LED所对应的端口标识
 */
void remove_led(unsigned int port)
{
    // 遍历LED数组，寻找与指定端口匹配的LED
    for (int i = 0; i < led_count; ++i)
    {
        if (leds[i].port == port)
        {
            // 将找到的LED后的所有LED向前移动一位，实现删除效果
            while (i < led_count - 1)
            {
                leds[i] = leds[i + 1];
                ++i;
            }
            // 更新LED数量，完成删除操作
            --led_count;
            break;
        }
    }
}
// 设置LED状态
void set_led_state(unsigned int port, LedState state)
{
    for (int i = 0; i < led_count; ++i)
    {
        if (leds[i].port == port)
        {
            leds[i].state = state;
            // gpio_write(leds[i].port, state == LED_KEEP_ON ? 1 : 0);
            break;
        }
    }
}

// LED闪烁定时器函数
void led_blink_timer()
{
    for (int i = 0; i < led_count; ++i)
    {
        if (leds[i].state == LED_BLINK)
        {
            if (leds[i].blink_cnt == leds[i].blink_speed)
            {
                leds[i].blink_cnt = 0;
                leds[i].blink_flag ^= 1;
                gpio_write(leds[i].port, leds[i].blink_flag);
                // printf("blink_flag %d\n", leds[i].blink_flag);
            }
            leds[i].blink_cnt++;
        }
        else if (leds[i].state == LED_KEEP_OFF)
        {
            gpio_write(leds[i].port, 0);
        }
        else if (leds[i].state == LED_KEEP_ON)
        {
            gpio_write(leds[i].port, 1);
        }
    }
}

// 示例：初始化和使用
int main_test()
{
    // init_led(&leds[0], IO_PORTB_00, LED_BLINK_SLOW, LED_BLINK);
    add_led(IO_PORTB_01, LED_BLINK_FAST, LED_BLINK);
    add_led(IO_PORTB_02, LED_BLINK_SLOW, LED_KEEP_ON); // 初始化为常亮

    // 在主循环中调用led_blink_timer()
    while (1)
    {
        led_blink_timer();

        // 若要切换某个LED的状态，例如将第三个LED由常亮改为关闭
        set_led_state(IO_PORTB_02, LED_KEEP_OFF);

        // 其他任务...
    }

    return 0;
}