/**
 * @file boards/hellen/premium-quick-test/board_configuration.cpp
 *
 * Bench bring-up board for the Hellen mega-mcu-premium 176-pin megamodule,
 * see hellen_premium176_meta.h
 */

#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"
#include "board_overrides.h"

// module LEDs sink current (V33 -> resistor -> LED -> MCU pin), see
// LED_PIN_MODE=OM_INVERTED in board.mk; red LED1 is the critical-error LED
Gpio getCommsLedPin() {
	return Gpio::MMP176_LED3_BLUE;
}

Gpio getRunningLedPin() {
	return Gpio::MMP176_LED2_GREEN;
}

Gpio getWarningLedPin() {
	return Gpio::MMP176_LED4_YELLOW;
}

static void premiumQuickTestDefaultConfiguration() {
	// on-module LPS22HB barometer, bit-banged I2C on the module SCL/SDA pads
	engineConfiguration->lps25BaroSensorScl = Gpio::MMP176_I2C_SCL;
	engineConfiguration->lps25BaroSensorSda = Gpio::MMP176_I2C_SDA;

	// Redundant TPS and pedal on the muxed analog inputs (ADC_MUX_PIN=PH15
	// in board.mk). Each mux pair shares one MCU ADC pin, so primary and
	// secondary tracks stay on the same physical channel:
	//   TPS1 track 1 -> pad AIN1, track 2 -> pad AIN2 (both PB1)
	setTPS1Inputs(MMP176_AIN1_ANALOG, MMP176_AIN2_ANALOG);
	//   pedal track 1 -> pad AIN4, track 2 -> pad AIN3 (both PB0)
	setPPSInputs(MMP176_AIN4_ANALOG, MMP176_AIN3_ANALOG);

	// three on-module TJA1042T/3 transceivers
	engineConfiguration->canRxPin = Gpio::MMP176_CAN1_RX;
	engineConfiguration->canTxPin = Gpio::MMP176_CAN1_TX;
	engineConfiguration->can2RxPin = Gpio::MMP176_CAN2_RX;
	engineConfiguration->can2TxPin = Gpio::MMP176_CAN2_TX;
#if (EFI_CAN_BUS_COUNT >= 3)
	engineConfiguration->can3RxPin = Gpio::MMP176_CAN3_RX;
	engineConfiguration->can3TxPin = Gpio::MMP176_CAN3_TX;
#endif

	// on-module eMMC via SDMMC2 (EFI_SDC_DEVICE) - logging + USB MSD
	engineConfiguration->isSdCardEnabled = true;
}

// All three TJA1042 S pins share MMP176_CAN_STB with no pull resistor:
// firmware must actively drive it low or the transceivers stay in silent
// (listen-only) mode and never transmit
static OutputPin canStb;

static void premiumQuickTestInitHardware() {
	canStb.initPin("CAN_STB", Gpio::MMP176_CAN_STB);
	canStb.setValue(0);
}

// On-module LAN8720A RMII PHY, same MCU pins as Nucleo-F767:
// REF_CLK=PA1 MDIO=PA2 CRS_DV=PA7 MDC=PC1 RXD0=PC4 RXD1=PC5
// TX_EN=PG11 TXD0=PG13 TXD1=PG14, PHY nRST=PE11.
// AF must be set before the MAC driver probes the PHY, hence preHalInit
// (see nucleo_h743 for the same pattern).
static void premiumQuickTestPreHalInit() {
	efiSetPadMode("Ethernet",  Gpio::A1, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::A2, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::A7, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("Ethernet",  Gpio::C1, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::C4, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::C5, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("Ethernet", Gpio::G11, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet", Gpio::G13, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet", Gpio::G14, PAL_MODE_ALTERNATE(0xb));

	// release PHY reset (no pull resistor on the net - must be driven)
	efiSetPadMode("Ethernet PHY nRST", Gpio::E11, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPad(GPIOE, 11);

	// On-module eMMC on 8-bit SDMMC2, AF11 for CK/CMD/D0/D1/D3 and AF10
	// for the PB/PC data lines (see hellen_premium176_meta.h for the map)
	efiSetPadMode("eMMC",  Gpio::D6, PAL_MODE_ALTERNATE(11) | PAL_STM32_OSPEED_HIGHEST); // CK
	efiSetPadMode("eMMC",  Gpio::D7, PAL_MODE_ALTERNATE(11) | PAL_STM32_OSPEED_HIGHEST); // CMD
	efiSetPadMode("eMMC",  Gpio::G9, PAL_MODE_ALTERNATE(11) | PAL_STM32_OSPEED_HIGHEST); // D0
	efiSetPadMode("eMMC", Gpio::G10, PAL_MODE_ALTERNATE(11) | PAL_STM32_OSPEED_HIGHEST); // D1
	efiSetPadMode("eMMC",  Gpio::B3, PAL_MODE_ALTERNATE(10) | PAL_STM32_OSPEED_HIGHEST); // D2
	efiSetPadMode("eMMC", Gpio::G12, PAL_MODE_ALTERNATE(11) | PAL_STM32_OSPEED_HIGHEST); // D3
	efiSetPadMode("eMMC",  Gpio::B8, PAL_MODE_ALTERNATE(10) | PAL_STM32_OSPEED_HIGHEST); // D4
	efiSetPadMode("eMMC",  Gpio::B9, PAL_MODE_ALTERNATE(10) | PAL_STM32_OSPEED_HIGHEST); // D5
	efiSetPadMode("eMMC",  Gpio::C6, PAL_MODE_ALTERNATE(10) | PAL_STM32_OSPEED_HIGHEST); // D6
	efiSetPadMode("eMMC",  Gpio::C7, PAL_MODE_ALTERNATE(10) | PAL_STM32_OSPEED_HIGHEST); // D7

	// hold eMMC RST_n high (JEDEC default has the reset function disabled,
	// high is the safe do-nothing level either way)
	efiSetPadMode("eMMC RST_n", Gpio::I3, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPad(GPIOI, 3);
}

void setup_custom_board_overrides() {
	custom_board_preHalInit = premiumQuickTestPreHalInit;
	custom_board_InitHardware = premiumQuickTestInitHardware;
	custom_board_DefaultConfiguration = premiumQuickTestDefaultConfiguration;
}
