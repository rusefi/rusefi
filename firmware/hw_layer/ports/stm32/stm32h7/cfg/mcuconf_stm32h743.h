/*
 * Memory attributes settings.
 */
#ifndef STM32_NOCACHE_ENABLE
#define STM32_NOCACHE_ENABLE                TRUE
#endif
#define STM32_NOCACHE_MPU_REGION            MPU_REGION_6
#define STM32_NOCACHE_RBAR                  0x30040000U
#define STM32_NOCACHE_RASR                  MPU_RASR_SIZE_16K

/*
 * PWR system settings.
 * Reading STM32 Reference Manual is required, settings in PWR_CR3 are
 * very critical.
 * Register constants are taken from the ST header.
 */
#define STM32_VOS                           STM32_VOS_SCALE1
#define STM32_PWR_CR1                       (PWR_CR1_SVOS_1 | PWR_CR1_SVOS_0)
#define STM32_PWR_CR2                       (PWR_CR2_BREN)
#define STM32_PWR_CR3                       (PWR_CR3_LDOEN | PWR_CR3_USB33DEN)
#define STM32_PWR_CPUCR                     0

/*
 * Clock tree static settings.
 * Reading STM32 Reference Manual is required.
 */
#define STM32_HSI_ENABLED                   TRUE
#define STM32_LSI_ENABLED                   TRUE
#define STM32_CSI_ENABLED                   TRUE
#define STM32_HSI48_ENABLED                 TRUE
#define STM32_HSE_ENABLED                   TRUE
// see RUSEFI_STM32_LSE_WAIT_MAX
#define STM32_LSE_ENABLED                   TRUE
#define STM32_HSIDIV                        STM32_HSIDIV_DIV1

/*
 * PLLs static settings.
 * Reading STM32 Reference Manual is required.
 *
 * Configured to use HSI to generate 400mhz, but we will reconfigure to use HSE
 * with appropriate PLLx_DIVM values for the detected external frequency.
 * See osc_detector.cpp
 */
#define STM32_PLLSRC                        STM32_PLLSRC_HSI_CK
#define STM32_PLLCFGR_MASK                  ~0

/* PLL1 output clock is 800 MHz */
#define STM32_PLL1_ENABLED                  TRUE
#define STM32_PLL1_P_ENABLED                TRUE
#define STM32_PLL1_Q_ENABLED                TRUE
#define STM32_PLL1_R_ENABLED                TRUE
#define STM32_PLL1_DIVM_VALUE               32
#define STM32_PLL1_DIVN_VALUE               400
#define STM32_PLL1_FRACN_VALUE              0
#define STM32_PLL1_DIVP_VALUE               2
#define STM32_PLL1_DIVQ_VALUE               4
#define STM32_PLL1_DIVR_VALUE               2

/* PLL2 output clock is 960 MHz */
#define STM32_PLL2_ENABLED                  TRUE
#define STM32_PLL2_P_ENABLED                TRUE
#define STM32_PLL2_Q_ENABLED                TRUE
#define STM32_PLL2_R_ENABLED                TRUE
#define STM32_PLL2_DIVM_VALUE               32
#define STM32_PLL2_DIVN_VALUE               480
#define STM32_PLL2_FRACN_VALUE              0
#define STM32_PLL2_DIVP_VALUE               10
#define STM32_PLL2_DIVQ_VALUE               12
#define STM32_PLL2_DIVR_VALUE               20

/* PLL3 output clock is 960 MHz */
#define STM32_PLL3_ENABLED                  TRUE
#define STM32_PLL3_P_ENABLED                TRUE
#define STM32_PLL3_Q_ENABLED                TRUE
#define STM32_PLL3_R_ENABLED                TRUE
#define STM32_PLL3_DIVM_VALUE               32
#define STM32_PLL3_DIVN_VALUE               480
#define STM32_PLL3_FRACN_VALUE              0
#define STM32_PLL3_DIVP_VALUE               2
#define STM32_PLL3_DIVQ_VALUE               20
#define STM32_PLL3_DIVR_VALUE               12

/*
 * Core clocks dynamic settings (can be changed at runtime).
 * Reading STM32 Reference Manual is required.
 */
#define STM32_SW                            STM32_SW_PLL1_P_CK
// see RUSEFI_STM32_LSE_WAIT_MAX_RTCSEL
#define STM32_RTCSEL                        STM32_RTCSEL_LSE_CK
#define STM32_D1CPRE                        STM32_D1CPRE_DIV1
#define STM32_D1HPRE                        STM32_D1HPRE_DIV2
#define STM32_D1PPRE3                       STM32_D1PPRE3_DIV2
#define STM32_D2PPRE1                       STM32_D2PPRE1_DIV2
#define STM32_D2PPRE2                       STM32_D2PPRE2_DIV2
#define STM32_D3PPRE4                       STM32_D3PPRE4_DIV2

/*
 * Peripherals clocks static settings.
 * Reading STM32 Reference Manual is required.
 */
#define STM32_MCO1SEL                       STM32_MCO1SEL_HSI_CK
#define STM32_MCO1PRE_VALUE                 4
#define STM32_MCO2SEL                       STM32_MCO2SEL_SYS_CK
#define STM32_MCO2PRE_VALUE                 4
#define STM32_TIMPRE_ENABLE                 TRUE
#define STM32_HRTIMSEL                      0
#define STM32_STOPKERWUCK                   0
#define STM32_STOPWUCK                      0
#define STM32_RTCPRE_VALUE                  8
#define STM32_CKPERSEL                      STM32_CKPERSEL_HSE_CK
#define STM32_SDMMCSEL                      STM32_SDMMCSEL_PLL2_R_CK
#define STM32_QSPISEL                       STM32_QSPISEL_HCLK
#define STM32_FMCSEL                        STM32_QSPISEL_HCLK
#define STM32_SWPSEL                        STM32_SWPSEL_PCLK1
#define STM32_FDCANSEL                      STM32_FDCANSEL_PLL2_Q_CK
#define STM32_DFSDM1SEL                     STM32_DFSDM1SEL_PCLK2
#define STM32_SPDIFSEL                      STM32_SPDIFSEL_PLL1_Q_CK
#define STM32_SPI45SEL                      STM32_SPI45SEL_PLL2_Q_CK
#define STM32_SPI123SEL                     STM32_SPI123SEL_PLL2_P_CK
#define STM32_SAI23SEL                      STM32_SAI23SEL_PLL1_Q_CK
#define STM32_SAI1SEL                       STM32_SAI1SEL_PLL1_Q_CK
#define STM32_LPTIM1SEL                     STM32_LPTIM1SEL_PCLK1
#define STM32_CECSEL                        STM32_CECSEL_LSE_CK
#define STM32_USBSEL                        STM32_USBSEL_PLL3_Q_CK
#define STM32_I2C123SEL                     STM32_I2C123SEL_PCLK1
#define STM32_RNGSEL                        STM32_RNGSEL_HSI48_CK
#define STM32_USART16SEL                    STM32_USART16SEL_PLL2_Q_CK
#define STM32_USART234578SEL                STM32_USART234578SEL_PLL2_Q_CK
#define STM32_SPI6SEL                       STM32_SPI6SEL_PLL2_Q_CK
#define STM32_SAI4BSEL                      STM32_SAI4BSEL_PLL1_Q_CK
#define STM32_SAI4ASEL                      STM32_SAI4ASEL_PLL1_Q_CK
#define STM32_ADCSEL                        STM32_ADCSEL_PLL3_R_CK
#define STM32_LPTIM345SEL                   STM32_LPTIM345SEL_PCLK4
#define STM32_LPTIM2SEL                     STM32_LPTIM2SEL_PCLK4
#define STM32_I2C4SEL                       STM32_I2C4SEL_PCLK4
#define STM32_LPUART1SEL                    STM32_LPUART1SEL_PCLK4

/*
 * USB driver system settings.
 */
#define STM32_USB_USE_OTG1                  TRUE
#define STM32_USB_USE_OTG2                  FALSE
#define STM32_USB_OTG1_RX_FIFO_SIZE         512
#define STM32_USB_OTG2_RX_FIFO_SIZE         1024
#define STM32_USB_HOST_WAKEUP_DURATION      2