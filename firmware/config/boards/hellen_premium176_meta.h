// hellen_premium176_meta.h
//
//
// MMP176 = MegaModule Premium, 176-pin package.

#pragma once

// Low-side injector outputs
#define MMP176_INJ1 G7
#define MMP176_INJ2 G8
#define MMP176_INJ3 D11
#define MMP176_INJ4 D10
#define MMP176_INJ5 H7
#define MMP176_INJ6 F15
#define MMP176_INJ7 F13
#define MMP176_INJ8 F14

// Ignition outputs (buffered on-module: MCU net IGNx drives pad OUT_IGNx)
#define MMP176_IGN1 F2
#define MMP176_IGN2 I11
#define MMP176_IGN3 I10
#define MMP176_IGN4 I9
#define MMP176_IGN5 C13
#define MMP176_IGN6 E3
#define MMP176_IGN7 I4
#define MMP176_IGN8 G15

#define MMP176_OUT_PWM1 B7
#define MMP176_OUT_PWM2 A10
// NOTE: module schematic PDF annotates PWM3 as PH6 - that is a schematic
// typo, PH6 is PWM13. The PCB netlist places PWM3 on PB11.
#define MMP176_OUT_PWM3 B11
#define MMP176_OUT_PWM4 C8
#define MMP176_OUT_PWM5 I2
#define MMP176_OUT_PWM6 D14
#define MMP176_OUT_PWM7 D15
#define MMP176_OUT_PWM8 I0
#define MMP176_OUT_PWM9 H12
#define MMP176_OUT_PWM10 H11
#define MMP176_OUT_PWM11 H10
#define MMP176_OUT_PWM12 H9
#define MMP176_OUT_PWM13 H6
#define MMP176_OUT_PWM14 I7
#define MMP176_OUT_PWM15 I6
#define MMP176_OUT_PWM16 I5

// General purpose IO pads
#define MMP176_IO1 D3
#define MMP176_IO2 H14
#define MMP176_IO3 D5
#define MMP176_IO4 G5
#define MMP176_IO5 D2
#define MMP176_IO6 H2
#define MMP176_IO7 G3
#define MMP176_IO8 G4
#define MMP176_IO9 H3
#define MMP176_IO10 F1
#define MMP176_IO11 F4

// Digital inputs. All are EXTI-capable; the ones on PFx also reach ADC3,
// which is the knock/high-speed ADC on Hellen F7 - channel noted per pin.
#define MMP176_DIN1 F0
#define MMP176_DIN2 I1
#define MMP176_DIN3 G2
// ADC3_IN9
#define MMP176_DIN4 F3
#define MMP176_DIN5 D4
// ADC3_IN15
#define MMP176_DIN6 F5
// ADC3_IN4
#define MMP176_DIN7 F6
// ADC3_IN5
#define MMP176_DIN8 F7
// ADC3_IN7; module net also tags this pad VR_CRK - intended crank VR input
// (VR conditioner lives on the frame, not on the module)
#define MMP176_DIN9 F9
#define MMP176_DIN10 E10
#define MMP176_DIN11 F11
#define MMP176_DIN12 F12
#define MMP176_DIN13 E9
// ADC3_IN8
#define MMP176_DIN14 F10
// ADC3_IN6
#define MMP176_DIN15 F8
// DIN16..DIN19 are tagged HALL_EINT in the module netlist - preferred
// hall-sensor crank/cam inputs
#define MMP176_DIN16 H8
#define MMP176_DIN17 E13
#define MMP176_DIN18 E14
#define MMP176_DIN19 E15

// Direct analog inputs
#define MMP176_AIN7 A0
#define MMP176_AIN7_ANALOG EFI_ADC_0
#define MMP176_AIN8 A3
#define MMP176_AIN8_ANALOG EFI_ADC_3
#define MMP176_AIN9 A4
#define MMP176_AIN9_ANALOG EFI_ADC_4
#define MMP176_AIN10 C2
#define MMP176_AIN10_ANALOG EFI_ADC_12
#define MMP176_AIN11 C3
#define MMP176_AIN11_ANALOG EFI_ADC_13
#define MMP176_AIN12 C0
#define MMP176_AIN12_ANALOG EFI_ADC_10

// Muxed analog inputs: AIN1..AIN6 are 2:1 multiplexed (three NC7SB3157,
// MCP6004-buffered) onto three MCU ADC pins, all switched together by
// MUX_CONTROL (PH15, use as ADC_MUX_PIN). Mux LOW selects the base channel,
// mux HIGH the +16 pseudo-channel (same convention as H144_IN_PPS2/TPS2).
//   PB1 (EFI_ADC_9):  LOW = AIN1, HIGH = AIN2
//   PB0 (EFI_ADC_8):  LOW = AIN4, HIGH = AIN3
//   PA6 (EFI_ADC_6):  LOW = AIN6, HIGH = AIN5
#define MMP176_AIN1_ANALOG EFI_ADC_9
// 9+16
#define MMP176_AIN2_ANALOG EFI_ADC_25
// 8+16
#define MMP176_AIN3_ANALOG EFI_ADC_24
#define MMP176_AIN4_ANALOG EFI_ADC_8
// 6+16
#define MMP176_AIN5_ANALOG EFI_ADC_22
#define MMP176_AIN6_ANALOG EFI_ADC_6
#define MMP176_MUX_CONTROL H15

// Battery/ignition voltage sense (divider on module)
#define MMP176_IN_VIGN EFI_ADC_5

// CAN buses (three on-module transceivers; pads are bus-side CANxH/CANxL)
#define MMP176_CAN1_RX D0
#define MMP176_CAN1_TX D1
#define MMP176_CAN2_RX B5
#define MMP176_CAN2_TX B6
#define MMP176_CAN3_RX A8
#define MMP176_CAN3_TX B4
// standby control shared by the CAN transceivers
#define MMP176_CAN_STB H13

// SPI buses exposed on module pads
#define MMP176_SPI2_CS B12
#define MMP176_SPI2_SCK B13
#define MMP176_SPI2_MISO B14
#define MMP176_SPI2_MOSI B15

#define MMP176_SPI3_CS A15
#define MMP176_SPI3_SCK C10
#define MMP176_SPI3_MISO C11
#define MMP176_SPI3_MOSI C12

// UARTs exposed on module pads
#define MMP176_UART3_TX D8
#define MMP176_UART3_RX D9
#define MMP176_UART8_TX E1
#define MMP176_UART8_RX E0

// I2C2 - exposed on SCL/SDA pads, shared with the on-module LPS22HB baro
#define MMP176_I2C_SCL H4
#define MMP176_I2C_SDA H5

// On-module LEDs (same GPIOs as the H144/mega144 LEDs)
#define MMP176_LED1_RED G0
#define MMP176_LED2_GREEN G1
#define MMP176_LED3_BLUE E7
#define MMP176_LED4_YELLOW E8

// Hellen power-management EN output (see setHellenMegaEnPin)
#define MMP176_PWR_EN G6

// --- on-module peripherals, fixed wiring, no frame access ---
// W25Q128 16MB NOR flash on true QUADSPI (AF9):
//   NCS=PB10 CLK=PB2 IO0=PC9 IO1=PD12 IO2=PE2 IO3=PD13
// LIS2DH12 accelerometer on SPI4: CS=PE4 SCK=PE12 MISO=PE5 MOSI=PE6
// LPS22HB barometer on I2C2 (PH4/PH5, shared with SCL/SDA pads)
// LAN8720A RMII Ethernet PHY, fully routed (same MCU pins as Nucleo-F767):
//   REF_CLK=PA1 MDIO=PA2 CRS_DV=PA7 MDC=PC1 RXD0=PC4 RXD1=PC5
//   TX_EN=PG11 TXD0=PG13 TXD1=PG14 nRST=PE11 (no pull - drive high);
//   frame-side pads are the magnetics pairs (ETH_TXP/N, ETH_RXP/N) + LED
// KLMAG1JETD 16GB eMMC, fully routed as 8-bit SDMMC2 (via series Rs):
//   CK=PD6 CMD=PD7 D0=PG9 D1=PG10 D2=PB3 D3=PG12 D4=PB8 D5=PB9
//   D6=PC6 D7=PC7 RST_n=PI3
// USB FS device on PA11/PA12, VBUS sensing on PA9 (OTG_FS_VBUS via divider)
// SWD on PA13/PA14; 8MHz crystal on PH0/PH1; wakeup input on PI8
