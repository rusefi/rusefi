
#include "pch.h"
#include "defaults.h"
#include "smart_gpio.h"
#include "drivers/gpio/l9779.h"

// PB14 is error LED, configured in board.mk
Gpio getCommsLedPin() {
	// this board has LEDs
	return Gpio::Unassigned;
}

Gpio getWarningLedPin() {
	// this board has LEDs
	return Gpio::Unassigned;
}

Gpio getRunningLedPin() {
	// this board has LEDs
	return Gpio::Unassigned;
}

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::L9779_OUT_1;
	engineConfiguration->injectionPins[1] = Gpio::L9779_OUT_2;
	engineConfiguration->injectionPins[2] = Gpio::L9779_OUT_3;
	engineConfiguration->injectionPins[3] = Gpio::L9779_OUT_4;

	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::L9779_IGN_1;
	engineConfiguration->ignitionPins[1] = Gpio::L9779_IGN_2;
	/* Two following has no IGBT populated, wasted spark is used */
	engineConfiguration->ignitionPins[2] = Gpio::L9779_IGN_3;
	engineConfiguration->ignitionPins[3] = Gpio::L9779_IGN_4;

	engineConfiguration->ignitionPinMode = OM_DEFAULT;
}

static void setupEtb() {
	// TLE9201 driver
	// This chip has three control pins:
	// DIR - sets direction of the motor
	// PWM - pwm control (enable high, coast low)
	// DIS - disables motor (enable low)

	// PWM pin
	engineConfiguration->etbIo[0].controlPin = Gpio::B14;
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = Gpio::B15;
//	// Disable pin todo clarify if we have it?
//	engineConfiguration->etbIo[0].disablePin = Gpio::C8;

	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}

/**
 * @brief   Board-specific configuration defaults.

 */
void setBoardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	/* SPI1 is used for communication with L9779 */
	engineConfiguration->is_enabled_spi_1 = true;

	engineConfiguration->spi1mosiPin = Gpio::E15;
	engineConfiguration->spi1misoPin = Gpio::E14;
	engineConfiguration->spi1sckPin = Gpio::E13;

  engineConfiguration->triggerInputPins[0] = Gpio::F8;
  engineConfiguration->camInputs[0] = Gpio::B9;

// todo	engineConfiguration->clt.adcChannel = EFI_ADC_; // ADC3 PF5
// todo	engineConfiguration->iat.adcChannel = EFI_ADC_; // ADC3 PF6
// todo	engineConfiguration->map.sensor.hwChannel = EFI_ADC_;

	// ?k high side/?k low side = ? ratio divider todo is the value below right?
  engineConfiguration->analogInputDividerCoefficient = 2.0f;

//	todo engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8; // 5.835

//	engineConfiguration->vbattAdcChannel = EFI_ADC_;
	engineConfiguration->adcVcc = 3.3f;

  setTPS1Inputs(EFI_ADC_12, EFI_ADC_13);

  setPPSInputs(EFI_ADC_10, EFI_ADC_11);
}

void setBoardConfigOverrides() {
	//CAN 1 bus overwrites
	engineConfiguration->canRxPin = Gpio::G0;
	engineConfiguration->canTxPin = Gpio::G1;
	setupEtb();
}

static struct l9779_config l9779_cfg = {
	.spi_bus = &SPID1,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		.ssport = GPIOE,
		.sspad = 12,
		.cr1 =
			SPI_CR1_16BIT_MODE |
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			SPI_CR1_LSBFIRST |	//LSB first
			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	// div = 16
			SPI_CR1_MSTR |
			SPI_CR1_CPHA |
			0,
		.cr2 = SPI_CR2_16BIT_MODE
	},
	.direct_gpio = {
		/* ignition pre-drivers */
		[0] = {.port = GPIOF, .pad = 12},
		[1] = {.port = GPIOF, .pad = 13},
		[2] = {.port = GPIOF, .pad = 14},
		[3] = {.port = GPIOF, .pad = 15},
		/* IN1 .. IN7 */
		[4] = {.port = GPIOE, .pad = 8},
		[5] = {.port = GPIOE, .pad = 9},
		[6] = {.port = GPIOE, .pad = 10},
		[7] = {.port = GPIOE, .pad = 11},
		[8] = {.port = GPIOG, .pad = 5},
		[9] = {.port = GPIOG, .pad = 6},
		[10] = {.port = GPIOG, .pad = 7},
	},
	/* PWM signal */
	.pwm_gpio = {.port = NULL, .pad = 0}
};

static void board_init_ext_gpios()
{
	int ret;

	ret = l9779_add(Gpio::L9779_IGN_1, 0, &l9779_cfg);
	if (ret < 0) {
		/* error */
	}
}

/**
 * @brief Board-specific initialization code.
 * @todo  Add your board-specific code, if any.
 */
void boardInit(void)
{
	board_init_ext_gpios();
}

static Gpio OUTPUTS[] = {
	Gpio::L9779_OUT_4, // Injector 1
	Gpio::L9779_OUT_3, // Injector 2
//	Gpio::L9779_OUT_2, // Injector 3
//	Gpio::L9779_OUT_1, // Injector 4
//	Gpio::L9779_OUT_6, // Oxygen sensor 1 heater
//	Gpio::L9779_OUT_5, // EVAP solenoid control
//	Gpio::L9779_OUT_7, // Oxygen sensor 2 heater
//	Gpio::L9779_IGN_1, // Coil 1 (< +2.5v) / Coils 1,4
//	Gpio::L9779_IGN_3, // Coil 3  (< +2.5v) / Coils 2,4
//	Gpio::L9779_OUT_17, // Air compressor control
//	Gpio::L9779_OUT_14, // Secondary starter relay
//	Gpio::L9779_OUT_15, // FAN 1 relay
//	Gpio::L9779_OUT_16, // FAN 2 relay
//	Gpio::L9779_OUT_13, // Fuel pump relay
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

int getBoardMetaDcOutputsCount() {
    return 1;
}
