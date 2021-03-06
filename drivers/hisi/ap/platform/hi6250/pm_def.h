#ifndef __PM_DEF_H__
#define __PM_DEF_H__ 
#define AP_WAKE_INT_MASK (BIT(SOC_SCTRL_SCINT_MASK_gpio_22_int_START) | \
                      BIT(SOC_SCTRL_SCINT_MASK_gpio_23_int_START) | \
                      BIT(SOC_SCTRL_SCINT_MASK_gpio_24_int_START) | \
                      BIT(SOC_SCTRL_SCINT_MASK_gpio_25_int_START) | \
                      BIT(SOC_SCTRL_SCINT_MASK_gpio_26_int_START) | \
                      BIT(SOC_SCTRL_SCINT_MASK_gpio_27_int_START) | \
                      BIT(SOC_SCTRL_SCINT_MASK_rtc_int_START) | \
                      BIT(SOC_SCTRL_SCINT_MASK_rtc1_int_START) | \
                      BIT(SOC_SCTRL_SCINT_MASK_timer00_int_START) | \
                      BIT(SOC_SCTRL_SCINT_MASK_timer11_int_START) | \
                      BIT(SOC_SCTRL_SCINT_MASK_timer40_int_START) | \
                      BIT(SOC_SCTRL_SCINT_MASK_timer50_int_START) | \
                             BIT(SOC_SCTRL_SCINT_MASK_timer71_int_START))
#define AP_WAKE_INT_MASK1 (BIT(SOC_SCTRL_SCINT_MASK1_ao_gpio6_int_wk_msk_START) | \
        BIT(SOC_SCTRL_SCINT_MASK1_ao_gpio7_int_wk_msk_START))
#define IOMCU_WAKE_INT_MASK (BIT(SOC_SCTRL_SCINT_MASK_intr_iomcu_wdog_START) | \
                      BIT(SOC_SCTRL_SCINT_MASK_intr_wakeup_iomcu_START))
#define MODEM_INT_MASK (0x0000f210)
#define MODEM_DRX_INT_MASK (0x0000003f)
#define HIFI_WAKE_INT_MASK (BIT(SOC_SCTRL_SCINT_MASK_intr_asp_ipc_arm_START) | \
                      BIT(SOC_SCTRL_SCINT_MASK_intr_asp_watchdog_START))
#endif
