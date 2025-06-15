/*
 * hellen_meta.h
 *
 * @date Oct 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#pragma once
#include "hellen_mm64_meta.h"
#include "hellen_mm100_meta.h"
#include "hellen_mm176_meta.h"

// stm32 UART8
#define H144_UART8_RX E0
#define H144_UART8_TX E1

// stm32 UART2
#define H144_UART2_RX D6
#define H144_UART2_TX D5

#define H144_LED1_RED Gpio::G0
#define H144_LED2_GREEN Gpio::G1
#define H144_LED3_BLUE Gpio::E7
#define H144_LED4_YELLOW Gpio::E8

#define H144_USB1ID Gpio::A10

#define H176_MCU_MEGA_LED1_RED Gpio::G0

#define H144_LS_1 G7
#define H144_LS_2 G8
#define H144_LS_3 D11
#define H144_LS_4 D10
#define H144_LS_5 D9
#define H144_LS_6 F12
#define H144_LS_7 F13
#define H144_LS_8 F14

#define H144_IGN_1 C13
#define H144_IGN_2 E5
#define H144_IGN_3 E4
#define H144_IGN_4 E3
#define H144_IGN_5 E2
#define H144_IGN_6 B8
#define H144_IGN_7 B9
#define H144_IGN_8 E6

#define H176_IGN_1 Gpio::I8
#define H176_IGN_2 Gpio::E5
#define H176_IGN_3 Gpio::E4
#define H176_IGN_4 Gpio::E3
#define H176_IGN_5 Gpio::E2

#define H144_OUT_PWM1 D13
#define H144_OUT_PWM2 C6
#define H144_OUT_PWM3 C7
#define H144_OUT_PWM4 C8
#define H144_OUT_PWM5 C9
#define H144_OUT_PWM6 D14
#define H144_OUT_PWM7 D15
#define H144_OUT_PWM8 D12


// also IO1/IO5 on non-mega rev 0.6
#define H144_GP_IO1 D4
// also IO2/IO5 on non-mega rev 0.6
#define H144_GP_IO2 D7
#define H144_GP_IO3 G10
#define H144_GP_IO4 G9
#define H144_GP_IO5 G15
#define H144_GP_IO6 D8
#define H144_GP_IO7 F15
// OUT_PWM_EN on kicad symbol
#define H144_GP8 Gpio::E10
#define H144_GP9 Gpio::F2

#define H144_OUT_IO1 D3
// TIM1_CH2
#define H144_OUT_IO2 A9
#define H144_OUT_IO3 G14
#define H144_OUT_IO4 G5
#define H144_OUT_IO5 D2
#define H144_OUT_IO6 G11
#define H144_OUT_IO7 G3
#define H144_OUT_IO8 G4
#define H144_OUT_IO9 G13
#define H144_OUT_IO10 G12
#define H144_OUT_IO11 G2
// TIM1_CH1
#define H144_OUT_IO12 A8
#define H144_OUT_IO13 G6

// A24
#define H144_IN_CRANK B1
#define H144_IN_CRANK_ANALOG EFI_ADC_9
// A19
#define H144_IN_CAM A6
#define H144_IN_CAM_ANALOG EFI_ADC_6
// DIN5
#define H144_IN_VSS F11

// IN_CLT AIN11 PC2
#define H144_IN_CLT EFI_ADC_12
#define H144_IN_AT2 EFI_ADC_28
// IN_IAT AIN14 PC3
#define H144_IN_IAT EFI_ADC_13
#define H144_IN_IAT_DIGITAL C3
#define H144_IN_AT1 EFI_ADC_29

// IN_PPS AIN18 PA3
#define H144_IN_PPS EFI_ADC_3
// IN_TPS AIN17 PA4
#define H144_IN_TPS EFI_ADC_4
// VIGN PA5
#define H144_IN_VBATT EFI_ADC_5

// muxed
// 3+16
#define H144_IN_PPS2 EFI_ADC_19
// 4+16
#define H144_IN_TPS2 EFI_ADC_20

// IN_MAP1 AIN9 PC0
#define H144_IN_MAP1 EFI_ADC_10
#define H144_IN_MAP1_DIGITAL C0
// AIN10 PC1
#define H144_IN_MAP2 EFI_ADC_11
#define H144_IN_MAP2_DIGITAL C1
// AIN15 A15 PA2. Also ADC3#2 for knock
#define H144_IN_MAP3 EFI_ADC_2
#define H144_IN_MAP3_DIGITAL A2

// AIN7
#define H144_IN_RES1 F9

// AIN16
#define H144_IN_RES2 F10
#define H144_ADC_IN_RES2 ADC_CHANNEL_IN8

// AIN8
#define H144_IN_RES3 F8
#define H144_ADC_IN_RES3 ADC_CHANNEL_IN6


#define H144_IN_AUX1_ANALOG EFI_ADC_8
#define H144_IN_AUX1_DIGITAL B0
#define H144_IN_AUX2_ANALOG EFI_ADC_14
// fun fact: same input goes to TWO stm32 pins so that we can use it both as analog and ICU capture
//#define H144_IN_AUX2_DIGITAL C4
// TIM1_CH1
#define H144_IN_AUX2_DIGITAL E9

// AIN20 PA7
#define H144_IN_AUX3_ANALOG EFI_ADC_7
#define H144_IN_AUX3_DIGITAL A7
// mega-mcu aux3/4 is flipped comparing to H144_ORIGINAL_MCU_IN_D_AUX4
// AIN22 PC5
#define H144_IN_AUX4_ANALOG EFI_ADC_15
#define H144_IN_AUX4_DIGITAL C5

// A1 AIN1 no code to use ADC3 as analog, ADC3_IN9 for knock
#define H144_IN_SENS1 F3
// A2 AIN2 no code to use ADC3 as analog, ADC3_IN14 for knock
#define H144_IN_SENS2 F4
// A3 AIN3 no code to use ADC3 as analog, ADC3_IN15 for knock
#define H144_IN_SENS3 F5
// A4 AIN4 no code to use ADC3 as analog, ADC3_IN4 for knock
#define H144_IN_SENS4 F6

// IN_O2S AIN13 A13 PA0
#define H144_IN_O2S EFI_ADC_0
#define H144_IN_O2S_DIGITAL A0
// IN_O2S2 AIN12 PA1, also ADC3#1
#define H144_IN_O2S2 EFI_ADC_1
#define H144_IN_O2S2_DIGITAL A1

// same pin was used by IN_DIG8 before mega-mcu
#define H144_IN_D_1 E12
// same pin was used by IN_DIG9 before mega-mcu
#define H144_IN_D_2 E13
// same pin was used by IN_DIG10 before mega-mcu
#define H144_IN_D_3 E14
// same pin was used by IN_DIG11 before mega-mcu
#define H144_IN_D_4 E15

// A22
#define H144_ORIGINAL_MCU_IN_D_AUX3 C5
#define H144_ORIGINAL_MCU_IN_D_AUX3_ANALOG EFI_ADC_15
// it's a MESS see also H144_IN_AUX4_DIGITAL
// A20 AIN20
#define H144_ORIGINAL_MCU_IN_D_AUX4 A7

// CAN
#define H144_CAN_RX Gpio::D0
#define H144_CAN_TX Gpio::D1

// same pins 144 and 176
#define H176_CAN_RX H144_CAN_RX
#define H176_CAN_TX H144_CAN_TX

#define H176_IN_A16 Gpio::F10

#define H_SPI1_MOSI B5
#define H_SPI1_MISO B4
#define H_SPI1_SCK B3
#define H_SPI1_CS1 B6
#define H_SPI1_CS2 B7

// looks like same pins 144 and 176
#define H_SPI2_MOSI Gpio::B15
#define H_SPI2_MISO Gpio::B14
#define H_SPI2_SCK Gpio::B13
#define H_SPI2_CS Gpio::B12

#define H_SPI3_MOSI Gpio::C12
#define H_SPI3_MISO Gpio::C11
#define H_SPI3_SCK Gpio::C10
#define H_SPI3_CS Gpio::A15

#include "hellen_logic.h"
