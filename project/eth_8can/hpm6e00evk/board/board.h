#ifndef _HPM_BOARD_H
#define _HPM_BOARD_H
#include <stdio.h>
#include "hpm_common.h"
#include "hpm_soc.h"
#include "hpm_soc_feature.h"
#include "hpm_clock_drv.h"
#include "pinmux.h"

#define BOARD_NAME          "hpm6e00evk"
#define BOARD_CPU_FREQ      (600000000UL)

#ifndef BOARD_RUNNING_CORE
#define BOARD_RUNNING_CORE HPM_CORE0
#endif

/* uart section */
#if BOARD_RUNNING_CORE == HPM_CORE0
#define BOARD_CONSOLE_UART_BASE       HPM_UART0
#define BOARD_CONSOLE_UART_CLK_NAME   clock_uart0
#define BOARD_CONSOLE_UART_IRQ        IRQn_UART0
#else
#define BOARD_CONSOLE_UART_BASE       HPM_UART1
#define BOARD_CONSOLE_UART_CLK_NAME   clock_uart1
#define BOARD_CONSOLE_UART_IRQ        IRQn_UART1
#endif

#define BOARD_CONSOLE_UART_BAUDRATE (115200UL)
#define BOARD_APP_UART_BASE         BOARD_CONSOLE_UART_BASE

/* LED section */
#define BOARD_LED_ON_LEVEL            (0U)
#define BOARD_LED_OFF_LEVEL           (1U)

#define BOARD_R_GPIO_CTRL             HPM_GPIO0
#define BOARD_R_GPIO_INDEX            GPIO_DI_GPIOD
#define BOARD_R_GPIO_PIN              0

#define BOARD_G_GPIO_CTRL             HPM_GPIO0
#define BOARD_G_GPIO_INDEX            GPIO_DI_GPIOD
#define BOARD_G_GPIO_PIN              1

/* Console type */
#define CONSOLE_TYPE_UART           0
#define BOARD_CONSOLE_TYPE          CONSOLE_TYPE_UART

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void board_init(void);
void board_init_console(void);
void board_init_pmp(void);
void board_init_clock(void);
void board_init_led_pins(void);
uint32_t board_init_uart_clock(UART_Type *ptr);

#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* _HPM_BOARD_H */
