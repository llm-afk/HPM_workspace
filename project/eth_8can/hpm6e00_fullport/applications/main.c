/*
 * Copyright (c) 2021-2024 HPMicro
 *
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "rtt_board.h"

int main(void)
{
    rt_kprintf("\nHello, HPM6E80 is running on custom board!\n");
    
    app_init_led_pins();
    
    while(1){
        app_led_write(0, APP_LED_ON);
        app_led_write(1, APP_LED_OFF);
        rt_thread_mdelay(500);
        
        app_led_write(0, APP_LED_OFF);
        app_led_write(1, APP_LED_ON);
        rt_thread_mdelay(500);
        
        rt_kprintf("System heartbeat...\n");
    }
    
    return 0;
}
