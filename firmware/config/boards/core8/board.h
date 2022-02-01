/**
 * @file boards/subaru_eg33/board_io.c
 *
 * @date Feb 06, 2021
 * @author Andrey Gusakov, 2021
 */

#ifndef BOARD_IO_H
#define BOARD_IO_H

#undef EFI_USB_SERIAL
#define EFI_USB_SERIAL TRUE

#undef EFI_USB_AF
#define EFI_USB_AF 10U

#undef EFI_USB_SERIAL_DM
#define EFI_USB_SERIAL_DM GPIOA_11

#undef EFI_USB_SERIAL_DP
#define EFI_USB_SERIAL_DP GPIOA_12

#undef EFI_CONSOLE_TX_BRAIN_PIN
#define EFI_CONSOLE_TX_BRAIN_PIN	GPIOA_9

#undef EFI_CONSOLE_RX_BRAIN_PIN
#define EFI_CONSOLE_RX_BRAIN_PIN	GPIOA_10

#undef EFI_USE_OSC
#define EFI_USE_OSC TRUE

#undef EFI_CAN_SUPPORT
#define EFI_CAN_SUPPORT TRUE

#undef CAN_USE_SLEEP_MODE
#define CAN_USE_SLEEP_MODE  FALSE

#undef EFI_FILE_LOGGING
#define EFI_FILE_LOGGING	TRUE

#undef EFI_SDC_DEVICE
#define EFI_SDC_DEVICE	SDCD1

#undef HAL_USE_SDC
#define HAL_USE_SDC	TRUE

#undef HAL_USE_MMC_SPI
#define HAL_USE_MMC_SPI	FALSE


#undef EFI_ICU_INPUTS
#define EFI_ICU_INPUTS FALSE

#undef HAL_TRIGGER_USE_PAL
#define HAL_TRIGGER_USE_PAL TRUE

#undef EFI_LOGIC_ANALYZER
#define EFI_LOGIC_ANALYZER FALSE

#undef HAL_VSS_USE_PAL
#define HAL_VSS_USE_PAL TRUE

#undef LED_CRITICAL_ERROR_BRAIN_PIN
#define LED_CRITICAL_ERROR_BRAIN_PIN GPIOG_11

// Ignore USB VBUS pin (we're never a host, only a device)
#define BOARD_OTG_NOVBUSSENS TRUE

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#define STM32_LSEDRV                (3U << 3U)

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   300U


#define EFI_PIN_MODE_DEFAULT PIN_MODE_INPUT
#ifndef EFI_DR_DEFAULT
#define EFI_DR_DEFAULT PIN_PUPDR_PULLDOWN
#endif

// See https://github.com/rusefi/rusefi/issues/397
#define DEFAULT_GPIO_SPEED PIN_OSPEED_HIGH


/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

#define VAL_GPIO_MODER_ALL_DEFAULT  (EFI_PIN_MODE_DEFAULT(0) |           \
                                     EFI_PIN_MODE_DEFAULT(1) |           \
                                     EFI_PIN_MODE_DEFAULT(2) |           \
                                     EFI_PIN_MODE_DEFAULT(3) |           \
                                     EFI_PIN_MODE_DEFAULT(4) |         \
                                     EFI_PIN_MODE_DEFAULT(5) |   \
                                     EFI_PIN_MODE_DEFAULT(6) |           \
                                     EFI_PIN_MODE_DEFAULT(7) |           \
                                     EFI_PIN_MODE_DEFAULT(8) |           \
                                     EFI_PIN_MODE_DEFAULT(9) |           \
                                     EFI_PIN_MODE_DEFAULT(10) |          \
                                     EFI_PIN_MODE_DEFAULT(11) |          \
                                     EFI_PIN_MODE_DEFAULT(12) |          \
                                     EFI_PIN_MODE_DEFAULT(13) |          \
                                     EFI_PIN_MODE_DEFAULT(14) |          \
                                     EFI_PIN_MODE_DEFAULT(15))

#define VAL_GPIO_OTYPER_ALL_DEFAULT (PIN_OTYPE_PUSHPULL(0) |   \
                                     PIN_OTYPE_PUSHPULL(1) |   \
                                     PIN_OTYPE_PUSHPULL(2) |   \
                                     PIN_OTYPE_PUSHPULL(3) |   \
                                     PIN_OTYPE_PUSHPULL(4) |   \
                                     PIN_OTYPE_PUSHPULL(5) |   \
                                     PIN_OTYPE_PUSHPULL(6) |   \
                                     PIN_OTYPE_PUSHPULL(7) |   \
                                     PIN_OTYPE_PUSHPULL(8) |   \
                                     PIN_OTYPE_PUSHPULL(9) |   \
                                     PIN_OTYPE_PUSHPULL(10) |  \
                                     PIN_OTYPE_PUSHPULL(11) |  \
                                     PIN_OTYPE_PUSHPULL(12) |  \
                                     PIN_OTYPE_PUSHPULL(13) |  \
                                     PIN_OTYPE_PUSHPULL(14) |  \
                                     PIN_OTYPE_PUSHPULL(15))

#define VAL_GPIO_OSPEEDR_ALL_DEFAULT (DEFAULT_GPIO_SPEED(0) |          \
                                     DEFAULT_GPIO_SPEED(1) |          \
                                     DEFAULT_GPIO_SPEED(2) |          \
                                     DEFAULT_GPIO_SPEED(3) |          \
                                     DEFAULT_GPIO_SPEED(4) |         \
                                     DEFAULT_GPIO_SPEED(5) |  \
                                     DEFAULT_GPIO_SPEED(6) |          \
                                     DEFAULT_GPIO_SPEED(7) |          \
                                     DEFAULT_GPIO_SPEED(8) |          \
                                     DEFAULT_GPIO_SPEED(9) |          \
                                     DEFAULT_GPIO_SPEED(10) |         \
                                     DEFAULT_GPIO_SPEED(11) |         \
                                     DEFAULT_GPIO_SPEED(12) |          \
                                     DEFAULT_GPIO_SPEED(13) |          \
                                     DEFAULT_GPIO_SPEED(14) |          \
                                     DEFAULT_GPIO_SPEED(15))

#define VAL_GPIO_ODR_ALL_DEFAULT    0

#define VAL_GPIO_PUPDR_ALL_DEFAULT (EFI_DR_DEFAULT(0) |        \
                                     EFI_DR_DEFAULT(1) |       \
                                     EFI_DR_DEFAULT(2) |       \
                                     EFI_DR_DEFAULT(3) |       \
                                     EFI_DR_DEFAULT(4) |       \
                                     EFI_DR_DEFAULT(5) |       \
                                     EFI_DR_DEFAULT(6) |       \
                                     EFI_DR_DEFAULT(7) |       \
                                     EFI_DR_DEFAULT(8) |       \
                                     EFI_DR_DEFAULT(9) |       \
                                     EFI_DR_DEFAULT(10) |      \
                                     EFI_DR_DEFAULT(11) |      \
                                     EFI_DR_DEFAULT(12) |      \
                                     EFI_DR_DEFAULT(13) |      \
                                     EFI_DR_DEFAULT(14) |      \
                                     EFI_DR_DEFAULT(15))

#define VAL_GPIO_AF_ALL_DEFAULT   (PIN_AFIO_AF(0, 0U) |          \
                                   PIN_AFIO_AF(1, 0U) |          \
                                   PIN_AFIO_AF(2, 0U) |          \
                                   PIN_AFIO_AF(3, 0U) |          \
                                   PIN_AFIO_AF(4, 0U) |          \
                                   PIN_AFIO_AF(5, 0U) |          \
                                   PIN_AFIO_AF(6, 0U) |          \
                                   PIN_AFIO_AF(7, 0U))


#define VAL_GPIOA_MODER             (EFI_PIN_MODE_DEFAULT(0) |         \
                                     EFI_PIN_MODE_DEFAULT(1) |           \
                                     EFI_PIN_MODE_DEFAULT(2) |           \
                                     EFI_PIN_MODE_DEFAULT(3) |           \
                                     EFI_PIN_MODE_DEFAULT(4) |       \
                                     EFI_PIN_MODE_DEFAULT(5) |        \
                                     EFI_PIN_MODE_DEFAULT(6) |        \
                                     EFI_PIN_MODE_DEFAULT(7) |        \
                                     EFI_PIN_MODE_DEFAULT(8) |           \
                                     EFI_PIN_MODE_DEFAULT(9) |        \
                                     EFI_PIN_MODE_DEFAULT(10) |  \
                                     EFI_PIN_MODE_DEFAULT(11) |  \
                                     EFI_PIN_MODE_DEFAULT(12) |  \
                                     EFI_PIN_MODE_DEFAULT(13) |      \
                                     EFI_PIN_MODE_DEFAULT(14) |      \
                                     EFI_PIN_MODE_DEFAULT(15))
#define VAL_GPIOA_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOA_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOA_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOA_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOA_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOA_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOB setup:
 */
#define VAL_GPIOB_MODER             (EFI_PIN_MODE_DEFAULT(0) |         \
                                     EFI_PIN_MODE_DEFAULT(1) |           \
                                     EFI_PIN_MODE_DEFAULT(2) |           \
                                     EFI_PIN_MODE_DEFAULT(3) |           \
                                     EFI_PIN_MODE_DEFAULT(4) |       \
                                     PIN_MODE_ALTERNATE(5) |        \
                                     PIN_MODE_ALTERNATE(6) |        \
                                     EFI_PIN_MODE_DEFAULT(7) |        \
                                     EFI_PIN_MODE_DEFAULT(8) |           \
                                     EFI_PIN_MODE_DEFAULT(9) |        \
                                     EFI_PIN_MODE_DEFAULT(10) |  \
                                     EFI_PIN_MODE_DEFAULT(11) |  \
                                     EFI_PIN_MODE_DEFAULT(12) |  \
                                     EFI_PIN_MODE_DEFAULT(13) |      \
                                     EFI_PIN_MODE_DEFAULT(14) |      \
                                     EFI_PIN_MODE_DEFAULT(15))
#define VAL_GPIOB_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOB_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOB_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOB_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(0, 0U) |        \
                                     PIN_AFIO_AF(1, 0U) |          \
                                     PIN_AFIO_AF(2, 0U) |          \
                                     PIN_AFIO_AF(3, 0U) |          \
                                     PIN_AFIO_AF(4, 0U) |          \
                                     PIN_AFIO_AF(5, 9U) |           \
                                     PIN_AFIO_AF(6, 9U))
#define VAL_GPIOB_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOC setup:
 */
#define VAL_GPIOC_MODER             (EFI_PIN_MODE_DEFAULT(0) |         \
                                     EFI_PIN_MODE_DEFAULT(1) |           \
                                     EFI_PIN_MODE_DEFAULT(2) |           \
                                     EFI_PIN_MODE_DEFAULT(3) |           \
                                     EFI_PIN_MODE_DEFAULT(4) |       \
                                     EFI_PIN_MODE_DEFAULT(5) |        \
                                     EFI_PIN_MODE_DEFAULT(6) |        \
                                     EFI_PIN_MODE_DEFAULT(7) |        \
                                     PIN_MODE_ALTERNATE(8) |           \
                                     PIN_MODE_ALTERNATE(9) |        \
                                     PIN_MODE_ALTERNATE(10) |  \
                                     PIN_MODE_ALTERNATE(11) |  \
                                     PIN_MODE_ALTERNATE(12) |  \
                                     EFI_PIN_MODE_DEFAULT(13) |      \
                                     EFI_PIN_MODE_DEFAULT(14) |      \
                                     EFI_PIN_MODE_DEFAULT(15))
#define VAL_GPIOC_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOC_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOC_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOC_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOC_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(8, 12U) |           \
                                     PIN_AFIO_AF(9, 12U) |           \
                                     PIN_AFIO_AF(10, 12U) |           \
                                     PIN_AFIO_AF(11, 12U) |           \
                                     PIN_AFIO_AF(12, 12U))

/*
 * GPIOD setup:
 */

#define VAL_GPIOD_MODER             (PIN_MODE_ALTERNATE(0) |         \
                                     PIN_MODE_ALTERNATE(1) |           \
                                     PIN_MODE_ALTERNATE(2) |           \
                                     EFI_PIN_MODE_DEFAULT(3) |           \
                                     EFI_PIN_MODE_DEFAULT(4) |       \
                                     EFI_PIN_MODE_DEFAULT(5) |        \
                                     EFI_PIN_MODE_DEFAULT(6) |        \
                                     EFI_PIN_MODE_DEFAULT(7) |        \
                                     EFI_PIN_MODE_DEFAULT(8) |           \
                                     EFI_PIN_MODE_DEFAULT(9) |        \
                                     EFI_PIN_MODE_DEFAULT(10) |  \
                                     EFI_PIN_MODE_DEFAULT(11) |  \
                                     EFI_PIN_MODE_DEFAULT(12) |  \
                                     EFI_PIN_MODE_DEFAULT(13) |      \
                                     EFI_PIN_MODE_DEFAULT(14) |      \
                                     EFI_PIN_MODE_DEFAULT(15))
#define VAL_GPIOD_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOD_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOD_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOD_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(0, 9U) |        \
                                     PIN_AFIO_AF(1, 9U) |          \
                                     PIN_AFIO_AF(2, 12U))
#define VAL_GPIOD_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOE setup:
 */
#define VAL_GPIOE_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOE_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOE_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOE_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOE_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOE_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOE_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOF setup:
 */
#define VAL_GPIOF_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOF_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOF_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOF_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOF_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOF_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOF_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOG setup:
 */
#define VAL_GPIOG_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOG_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOG_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOG_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOG_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOG_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOG_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOH setup:
 */
#define VAL_GPIOH_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOH_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOH_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOH_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOH_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOH_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOH_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOI setup:
 */
#define VAL_GPIOI_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOI_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOI_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOI_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOI_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOI_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOI_AFRH              VAL_GPIO_AF_ALL_DEFAULT

#endif /* BOARD_IO_H */
