#include "board.h"
#include "hpm_uart_drv.h"
#include "hpm_gpio_drv.h"
#include "pinmux.h"
#include "hpm_pmp_drv.h"
#include "hpm_clock_drv.h"
#include "hpm_sysctl_drv.h"
#include "hpm_pllctlv2_drv.h"
#include "hpm_pcfg_drv.h"
#include "hpm_debug_console.h"

#if defined(FLASH_XIP) && FLASH_XIP
__attribute__((section(".nor_cfg_option"), used)) const uint32_t option[4] = { 0xfcf90001, 0x00000007, 0x0, 0x0 };
#endif

void board_init_console(void)
{
    console_config_t cfg;
    init_uart_pins((UART_Type *) BOARD_CONSOLE_UART_BASE);
    
    /* Use standardized clock init */
    cfg.src_freq_in_hz = board_init_uart_clock((UART_Type *) BOARD_CONSOLE_UART_BASE);

    cfg.type = BOARD_CONSOLE_TYPE;
    cfg.base = (uint32_t) BOARD_CONSOLE_UART_BASE;
    cfg.baudrate = BOARD_CONSOLE_UART_BAUDRATE;

    if (status_success != console_init(&cfg)) {
        while (1);
    }
}

void board_init(void)
{
    board_init_clock();
    board_init_console();
    board_init_pmp();
}

void board_init_pmp(void)
{
    uint32_t start_addr;
    uint32_t end_addr;
    uint32_t length;
    pmp_entry_t pmp_entry[1];
    
    extern uint32_t __noncacheable_start__[];
    extern uint32_t __noncacheable_end__[];
    start_addr = (uint32_t) __noncacheable_start__;
    end_addr = (uint32_t) __noncacheable_end__;
    length = end_addr - start_addr;
    if (length > 0) {
        pmp_entry[0].pmp_addr = PMP_NAPOT_ADDR(start_addr, length);
        pmp_entry[0].pmp_cfg.val = PMP_CFG(READ_EN, WRITE_EN, EXECUTE_EN, ADDR_MATCH_NAPOT, REG_UNLOCK);
        pmp_entry[0].pma_addr = PMA_NAPOT_ADDR(start_addr, length);
        pmp_entry[0].pma_cfg.val = PMA_CFG(ADDR_MATCH_NAPOT, MEM_TYPE_MEM_NON_CACHE_BUF, AMO_EN);
        pmp_config(&pmp_entry[0], 1);
    }
}

void board_init_clock(void)
{
    uint32_t cpu0_freq = clock_get_frequency(clock_cpu0);
    if (cpu0_freq == PLLCTL_SOC_PLL_REFCLK_FREQ) {
        pllctlv2_xtal_set_rampup_time(HPM_PLLCTLV2, 32ul * 1000ul * 9u);
        sysctl_clock_set_preset(HPM_SYSCTL, 2);
    }
    /* Add basic clocks to group 0 */
    clock_add_to_group(clock_cpu0, 0);
    clock_add_to_group(clock_mchtmr0, 0);
    clock_add_to_group(clock_ahb0, 0);
    clock_add_to_group(clock_axif, 0);
    clock_add_to_group(clock_axis, 0);
    clock_add_to_group(clock_axic, 0);
    clock_add_to_group(clock_axin, 0);
    clock_add_to_group(clock_rom0, 0);
    clock_add_to_group(clock_xpi0, 0);
    clock_add_to_group(clock_gpio, 0);
    clock_connect_group_to_cpu(0, 0);

    pcfg_dcdc_set_voltage(HPM_PCFG, 1275);
    clock_set_source_divider(clock_cpu0, clk_src_pll0_clk0, 1);
    clock_set_source_divider(clock_mchtmr0, clk_src_osc24m, 1);
    clock_update_core_clock();
}

uint32_t board_init_uart_clock(UART_Type *ptr)
{
    uint32_t freq = 0U;
    if (ptr == HPM_UART0) {
        clock_add_to_group(clock_uart0, 0);
        freq = clock_get_frequency(clock_uart0);
    } else if (ptr == HPM_UART1) {
        clock_add_to_group(clock_uart1, 0);
        freq = clock_get_frequency(clock_uart1);
    }
    return freq;
}

void board_init_led_pins(void)
{
    init_led_pins_as_gpio();
    gpio_set_pin_output_with_initial(BOARD_R_GPIO_CTRL, BOARD_R_GPIO_INDEX, BOARD_R_GPIO_PIN, BOARD_LED_OFF_LEVEL);
    gpio_set_pin_output_with_initial(BOARD_G_GPIO_CTRL, BOARD_G_GPIO_INDEX, BOARD_G_GPIO_PIN, BOARD_LED_OFF_LEVEL);
}
