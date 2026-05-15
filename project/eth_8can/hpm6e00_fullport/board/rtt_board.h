/*
 * Copyright (c) 2021-2023 HPMicro
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _RTT_BOARD_H
#define _RTT_BOARD_H
#include "hpm_common.h"
#include "hpm_soc.h"
#include "board.h"

/* mchtimer section */
#define BOARD_MCHTMR_FREQ_IN_HZ (24000000UL)

/* gptmr as os_tick */
#define BOARD_OS_TIMER HPM_GPTMR0
#define BOARD_OS_TIMER_CH       1
#define BOARD_OS_TIMER_IRQ      IRQn_GPTMR0
#define BOARD_OS_TIMER_CLK_NAME (clock_gptmr0)

#define IRQn_PendSV IRQn_DEBUG0

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/* Core functions kept for RT-Thread boot */
void app_init_led_pins(void);
void app_led_write(uint32_t index, bool state);

#define APP_LED_ON  BOARD_LED_CTRL_FLAG_ON
#define APP_LED_OFF BOARD_LED_CTRL_FLAG_OFF

#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* _RTT_BOARD_H */
