/*
 * Copyright (c) 2021-2023 HPMicro
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "board.h"
#include "rtt_board.h"
#include "hpm_uart_drv.h"
#include "hpm_gpio_drv.h"
#include "hpm_mchtmr_drv.h"
#include "hpm_pmp_drv.h"
#include "assert.h"
#include "hpm_clock_drv.h"
#include "hpm_sysctl_drv.h"
#include <rthw.h>
#include <rtthread.h>
#include "hpm_dma_mgr.h"

extern int rt_hw_uart_init(void);
void os_tick_config(void);
void rtt_board_init(void);

extern uint32_t __heap_start__;
extern uint32_t __heap_end__;

void rt_hw_board_init(void)
{
    /* Initialize heap using linker symbols for robustness */
    rt_system_heap_init((void *)&__heap_start__, (void *)&__heap_end__);

    rtt_board_init();

    /* Call the RT-Thread Component Board Initialization */
    rt_components_board_init();
}

void os_tick_config(void)
{
    sysctl_config_clock(HPM_SYSCTL, clock_node_mchtmr0, clock_source_osc0_clk0, 1);
    sysctl_add_resource_to_cpu0(HPM_SYSCTL, sysctl_resource_mchtmr0);
    mchtmr_set_compare_value(HPM_MCHTMR, BOARD_MCHTMR_FREQ_IN_HZ / RT_TICK_PER_SECOND);
    enable_mchtmr_irq();
}

void rtt_board_init(void)
{
    board_init_clock();
    board_init_console();
    board_init_pmp();

    dma_mgr_init();

    /* Configure the OS Tick */
    os_tick_config();

    /* Initialize the UART driver */
    rt_hw_uart_init();

    /* Set console device */
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
}

void rt_hw_console_output(const char *str)
{
    while (*str != '\0')
    {
        /* Wait for TX slot avail */
        while (!(uart_get_status(BOARD_APP_UART_BASE) & uart_stat_tx_slot_avail));

        if (*str == '\n')
        {
            uart_send_byte(BOARD_APP_UART_BASE, '\r');
            while (!(uart_get_status(BOARD_APP_UART_BASE) & uart_stat_tx_slot_avail));
        }
        uart_send_byte(BOARD_APP_UART_BASE, *str++);
    }
}

ATTR_PLACE_AT(".isr_vector") void mchtmr_isr(void)
{
    HPM_MCHTMR->MTIMECMP = HPM_MCHTMR->MTIME + BOARD_MCHTMR_FREQ_IN_HZ / RT_TICK_PER_SECOND;
    rt_tick_increase();
}

void rt_hw_cpu_reset(void)
{
    HPM_PPOR->RESET_ENABLE |= (1UL << 31);
    HPM_PPOR->SOFTWARE_RESET = 1000U;
    while(1);
}
MSH_CMD_EXPORT_ALIAS(rt_hw_cpu_reset, reset, reset the board);
