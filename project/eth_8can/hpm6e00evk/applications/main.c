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
    
    while(1){
        rt_thread_mdelay(1000);
        rt_kprintf("System heartbeat...\n");
    }
    
    return 0;
}
