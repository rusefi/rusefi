/**
 * @file boards/S105/board.h
 *
 * @date Jan 03, 2023
 * @author Andrey Gusakov, 2023
 */

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Board identifier.
 */
#define BOARD_NAME              "Soate S105"

/*
 * Board frequencies.
 * NOTE: The LSE and HSE crystals are not fitted by default on the board.
 */
#define STM32_LSECLK            32768
#define STM32_HSECLK            25000000

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD               330

/*
 * IO pins assignments.
 */

#define GPIOC_OSC32_IN          14
#define GPIOC_OSC32_OUT         15

#define GPIOH_OSC_IN            0
#define GPIOH_OSC_OUT           1

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_25M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_50M(n)           (2U << ((n) * 2))
#define PIN_OSPEED_100M(n)          (3U << ((n) * 2))
#define PIN_PUDR_FLOATING(n)        (0U << ((n) * 2))
#define PIN_PUDR_PULLUP(n)          (1U << ((n) * 2))
#define PIN_PUDR_PULLDOWN(n)        (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << (((n) % 8) * 4))

/*
 * Port A setup.
 *
 * Values are taked from runnig ECU with stock FW
 * > mdw 0x40020000 10
 * 0x40020000: 2aa8ffff 00000000 0ffc0000 24000000 00005f00 00000000 00000000 00000000
 * 0x40020020: 00000000 00099770
 * TODO: decode
 */
#define VAL_GPIOA_MODER     (0x2aa8ffff)
#define VAL_GPIOA_OTYPER    (0x00000000)
#define VAL_GPIOA_OSPEEDR   (0x0ffc0000)
#define VAL_GPIOA_PUPDR     (0x24000000)
#define VAL_GPIOA_ODR       (0x00000000)
#define VAL_GPIOA_AFRL      (0x00000000)
#define VAL_GPIOA_AFRH      (0x00099770)

/*
 * Port B setup.
 *
 * Values are taked from runnig ECU with stock FW
 * > mdw 0x40020400 10
 * 0x40020400: 102a018f 00004300 200a00c0 00000010 00000108 00000000 00000000 00000000
 * 0x40020420: 00000000 00000133
 * TODO: decode
 *
 * PB4  - LDO2_EN               (Output, 1).
 *
 */
#define VAL_GPIOB_MODER     (0x102a018f)
#define VAL_GPIOB_OTYPER    (0x00004300)
#define VAL_GPIOB_OSPEEDR   (0x200a00c0)
#define VAL_GPIOB_PUPDR     (0x00000010)
#define VAL_GPIOB_ODR       (0x00000010)
#define VAL_GPIOB_AFRL      (0x00000000)
#define VAL_GPIOB_AFRH      (0x00000133)

/*
 * Port C setup.
 *
 * Values are taked from runnig ECU with stock FW
 * > mdw 0x40020800 10
 * 0x40020800: 0000afff 000000c0 0000a000 00004000 0000d400 00000000 00000000 00000000
 * 0x40020820: 22000000 00000000
 * TODO: decode
 */
#define VAL_GPIOC_MODER     (0x0000afff)
#define VAL_GPIOC_OTYPER    (0x000000c0)
#define VAL_GPIOC_OSPEEDR   (0x0000a000)
#define VAL_GPIOC_PUPDR     (0x00004000)
#define VAL_GPIOC_ODR       (0x00000000)
#define VAL_GPIOC_AFRL      (0x22000000)
#define VAL_GPIOC_AFRH      (0x00000000)

/*
 * Port D setup.
 *
 * Values are taked from runnig ECU with stock FW
 * > mdw 0x40020c00 10
 * 0x40020c00: 55551111 00003f55 00aa2022 00000000 00000000 00000010 00000000 00000000
 * 0x40020c20: 00000000 00000000
 * TODO: decode
 */
#define VAL_GPIOD_MODER     (0x55551111)
#define VAL_GPIOD_OTYPER    (0x00003f55)
#define VAL_GPIOD_OSPEEDR   (0x00aa2022)
#define VAL_GPIOD_PUPDR     (0x00000000)
#define VAL_GPIOD_ODR       (0x00000010)
#define VAL_GPIOD_AFRL      (0x00000000)
#define VAL_GPIOD_AFRH      (0x00000000)

/*
 * Port E setup.
 *
 * Values are taked from runnig ECU with stock FW
 * > mdw 0x40021000 10
 * 0x40021000: 11111501 00005471 22202802 00000000 00000000 00001010 00000000 00000000
 * 0x40021020: 00000000 00000000
 * TODO: decode
 */
#define VAL_GPIOE_MODER     (0x11111501)
#define VAL_GPIOE_OTYPER    (0x00005471)
#define VAL_GPIOE_OSPEEDR   (0x22202802)
#define VAL_GPIOE_PUPDR     (0x00000000)
#define VAL_GPIOE_ODR       (0x00001010)
#define VAL_GPIOE_AFRL      (0x00000000)
#define VAL_GPIOE_AFRH      (0x00000000)

/*
 * Port F setup.
 *
 * Pins are not present on LQFP100
 * All input with pull-up.
 */
#define VAL_GPIOF_MODER     0x00000000
#define VAL_GPIOF_OTYPER    0x00000000
#define VAL_GPIOF_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOF_PUPDR     (PIN_PUDR_PULLUP(0) |                           \
                             PIN_PUDR_PULLUP(1) |                           \
                             PIN_PUDR_PULLUP(2) |                           \
                             PIN_PUDR_PULLUP(3) |                           \
                             PIN_PUDR_PULLUP(4) |                           \
                             PIN_PUDR_PULLUP(5) |                           \
                             PIN_PUDR_PULLUP(6) |                           \
                             PIN_PUDR_PULLUP(7) |                           \
                             PIN_PUDR_PULLUP(8) |                           \
                             PIN_PUDR_PULLUP(9) |                           \
                             PIN_PUDR_PULLUP(10) |                          \
                             PIN_PUDR_PULLUP(11) |                          \
                             PIN_PUDR_PULLUP(12) |                          \
                             PIN_PUDR_PULLUP(13) |                          \
                             PIN_PUDR_PULLUP(14) |                          \
                             PIN_PUDR_PULLUP(15))
#define VAL_GPIOF_ODR       0xFFFFFFFF
#define VAL_GPIOF_AFRL      0x00000000
#define VAL_GPIOF_AFRH      0x00000000

/*
 * Port G setup.
 *
 * Pins are not present on LQFP100
 * All input with pull-up.
 */
#define VAL_GPIOG_MODER     0x00000000
#define VAL_GPIOG_OTYPER    0x00000000
#define VAL_GPIOG_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOG_PUPDR     (PIN_PUDR_PULLUP(0) |                           \
                             PIN_PUDR_PULLUP(1) |                           \
                             PIN_PUDR_PULLUP(2) |                           \
                             PIN_PUDR_PULLUP(3) |                           \
                             PIN_PUDR_PULLUP(4) |                           \
                             PIN_PUDR_PULLUP(5) |                           \
                             PIN_PUDR_PULLUP(6) |                           \
                             PIN_PUDR_PULLUP(7) |                           \
                             PIN_PUDR_PULLUP(8) |                           \
                             PIN_PUDR_PULLUP(9) |                           \
                             PIN_PUDR_PULLUP(10) |                          \
                             PIN_PUDR_PULLUP(11) |                          \
                             PIN_PUDR_PULLUP(12) |                          \
                             PIN_PUDR_PULLUP(13) |                          \
                             PIN_PUDR_PULLUP(14) |                          \
                             PIN_PUDR_PULLUP(15))
#define VAL_GPIOG_ODR       0xFFFFFFFF
#define VAL_GPIOG_AFRL      0x00000000
#define VAL_GPIOG_AFRH      0x00000000

/*
 * Port H setup.
 *
 * All input with pull-up except:
 * PH0  - GPIOH_OSC_IN          (input floating).
 * PH1  - GPIOH_OSC_OUT         (input floating).
 */
#define VAL_GPIOH_MODER     (PIN_MODE_INPUT(GPIOH_OSC_IN) |                 \
                             PIN_MODE_INPUT(GPIOH_OSC_OUT) |                \
                             PIN_MODE_INPUT(2) |                            \
                             PIN_MODE_INPUT(3) |                            \
                             PIN_MODE_INPUT(4) |                            \
                             PIN_MODE_INPUT(5) |                            \
                             PIN_MODE_INPUT(6) |                            \
                             PIN_MODE_INPUT(7) |                            \
                             PIN_MODE_INPUT(8) |                            \
                             PIN_MODE_INPUT(9) |                            \
                             PIN_MODE_INPUT(10) |                           \
                             PIN_MODE_INPUT(11) |                           \
                             PIN_MODE_INPUT(12) |                           \
                             PIN_MODE_INPUT(13) |                           \
                             PIN_MODE_INPUT(14) |                           \
                             PIN_MODE_INPUT(15))
#define VAL_GPIOH_OTYPER    0x00000000
#define VAL_GPIOH_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOH_PUPDR     (PIN_PUDR_FLOATING(GPIOH_OSC_IN) |              \
                             PIN_PUDR_FLOATING(GPIOH_OSC_OUT) |             \
                             PIN_PUDR_PULLUP(2) |                           \
                             PIN_PUDR_PULLUP(3) |                           \
                             PIN_PUDR_PULLUP(4) |                           \
                             PIN_PUDR_PULLUP(5) |                           \
                             PIN_PUDR_PULLUP(6) |                           \
                             PIN_PUDR_PULLUP(7) |                           \
                             PIN_PUDR_PULLUP(8) |                           \
                             PIN_PUDR_PULLUP(9) |                           \
                             PIN_PUDR_PULLUP(10) |                          \
                             PIN_PUDR_PULLUP(11) |                          \
                             PIN_PUDR_PULLUP(12) |                          \
                             PIN_PUDR_PULLUP(13) |                          \
                             PIN_PUDR_PULLUP(14) |                          \
                             PIN_PUDR_PULLUP(15))
#define VAL_GPIOH_ODR       0xFFFFFFFF
#define VAL_GPIOH_AFRL      0x00000000
#define VAL_GPIOH_AFRH      0x00000000

/*
 * Port I setup.
 *
 * Pins are not present on LQFP100
 * All input with pull-up.
 */
#define VAL_GPIOI_MODER     0x00000000
#define VAL_GPIOI_OTYPER    0x00000000
#define VAL_GPIOI_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOI_PUPDR     (PIN_PUDR_PULLUP(0) |                           \
                             PIN_PUDR_PULLUP(1) |                           \
                             PIN_PUDR_PULLUP(2) |                           \
                             PIN_PUDR_PULLUP(3) |                           \
                             PIN_PUDR_PULLUP(4) |                           \
                             PIN_PUDR_PULLUP(5) |                           \
                             PIN_PUDR_PULLUP(6) |                           \
                             PIN_PUDR_PULLUP(7) |                           \
                             PIN_PUDR_PULLUP(8) |                           \
                             PIN_PUDR_PULLUP(9) |                           \
                             PIN_PUDR_PULLUP(10) |                          \
                             PIN_PUDR_PULLUP(11) |                          \
                             PIN_PUDR_PULLUP(12) |                          \
                             PIN_PUDR_PULLUP(13) |                          \
                             PIN_PUDR_PULLUP(14) |                          \
                             PIN_PUDR_PULLUP(15))
#define VAL_GPIOI_ODR       0xFFFFFFFF
#define VAL_GPIOI_AFRL      0x00000000
#define VAL_GPIOI_AFRH      0x00000000

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
