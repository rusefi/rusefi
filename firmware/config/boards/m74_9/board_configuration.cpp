
#include "pch.h"
#include "defaults.h"
#include "board_overrides.h"
#include "smart_gpio.h"
#include "drivers/gpio/l9779.h"
#include "drivers/gpio/tle9201.h"

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
	/* physical wiring (see m74_9.yaml): OUT4 -> Injector 1, OUT3 -> Injector 2,
	 * OUT2 -> Injector 3, OUT1 -> Injector 4 */
	engineConfiguration->injectionPins[0] = Gpio::L9779_OUT_4;
	engineConfiguration->injectionPins[1] = Gpio::L9779_OUT_3;
	engineConfiguration->injectionPins[2] = Gpio::L9779_OUT_2;
	engineConfiguration->injectionPins[3] = Gpio::L9779_OUT_1;

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
static void m74_9_boardDefaultConfiguration() {
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

	/* SPI2 is used for TLE9201 ETB H-bridge diagnostics (see setupEtb) */
	engineConfiguration->is_enabled_spi_2 = true;
	engineConfiguration->spi2sckPin = Gpio::D1;  // PD1 AF6, ETC_SCK
	engineConfiguration->spi2misoPin = Gpio::D3; // PD3 AF6, ETC_SO
	engineConfiguration->spi2mosiPin = Gpio::D4; // PD4 AF6, ETC_SI

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

static void m74_9_boardConfigOverrides() {
	//CAN 1 bus overwrites
	engineConfiguration->canRxPin = Gpio::G0;
	engineConfiguration->canTxPin = Gpio::G1;
	setupEtb();
}

static struct l9779_config l9779_cfg = {
	.spi_bus = &SPID1,
	.spi_config = {
		.circular = false,
#if defined(_CHIBIOS_RT_CONF_VER_6_1_) || defined(AT32F4XX)
		.end_cb = nullptr,
#else
		.slave = false,
		.data_cb = nullptr,
		.error_cb = nullptr,
#endif
		.ssport = GPIOE,
		.sspad = 12,
		.cr1 =
			SPI_CR1_16BIT_MODE |
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			/* L9779WD-SPI: MSB-first 16-bit frames (datasheet 6.16.2), data
			 * latched on the rising edge of SCK and shifted on the falling
			 * edge. Mode 0 (CPOL=0, CPHA=0) and mode 1 (CPOL=0, CPHA=1) are
			 * both rejected by the chip with SPI_ERR (DO=0x8000) on every
			 * frame; Figure 73 shows SCK idling high, which together with the
			 * latch-on-rising requirement gives SPI mode 3 (CPOL=1, CPHA=1).
			 * Transfer clock stays under the 8 MHz limit (Table 53):
			 * div 32 = 4.5 MHz @ 144 MHz APB2 (div 16 = 9 MHz exceeds it). */
			((4 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	// div = 32
			SPI_CR1_MSTR |
			SPI_CR1_CPOL |
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

static struct tle9201_config tle9201_cfg = {
	.spi_bus = &SPID2,
	.spi_config = {
		.circular = false,
#if defined(_CHIBIOS_RT_CONF_VER_6_1_) || defined(AT32F4XX)
		.end_cb = nullptr,
#else
		.slave = false,
		.data_cb = nullptr,
		.error_cb = nullptr,
#endif
		/* TLE9201 ETB H-bridge: CS on PD0, GPIO-driven (idle high) */
		.ssport = GPIOD,
		.sspad = 0,
		.cr1 = TLE9201_CONFIG_CR1,
		.cr2 = TLE9201_CONFIG_CR2
	}
};

static void board_init_ext_gpios()
{
	int ret;

	/* L9779 only registers a gpiochip here (safe pre-OS); its thread and SPI
	 * start later via gpiochips_init(). TLE9201 must NOT be added here: its
	 * init() creates a thread (chThdCreateStatic), which hangs when called
	 * from boardInit() before chSysInit() - see m74_9_boardInitHardware. */
	ret = l9779_add(Gpio::L9779_IGN_1, 0, &l9779_cfg);
	if (ret < 0) {
		/* error */
	}
}

/**
 * @brief Called from halInit() BEFORE the kernel exists (chSysInit).
 * @details Only L9779 may be registered here: it just registers a gpiochip,
 * its thread starts later via gpiochips_init(). TLE9201 must NOT be added
 * here - its init() calls chThdCreateStatic() and hangs the board at power-on.
 */
void boardInit() {
	board_init_ext_gpios();
}

/**
 * @brief Board-specific init after the kernel is up.
 * @details Runs from custom_board_InitHardware (post chSysInit, before
 * initSpiModules). TLE9201::init() calls chThdCreateStatic(), which must not
 * run from boardInit() - boardInit() is called from halInit() before the
 * kernel exists and the board hangs at power-on. Same pattern as
 * hellen154hyundai_f7.
 */
static void m74_9_boardInitHardware() {
	/* TLE9201 ETB H-bridge: ETC_CS on PD0 */
	gpio_pin_markUsed(GPIOD, 0, "TLE9201 ETB CS");
	palSetPadMode(GPIOD, 0, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPad(GPIOD, 0);

	int ret = tle9201_add(0, &tle9201_cfg);
	efiPrintf("tle9201_add()=%d", ret);
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
void setup_custom_board_overrides() {
	custom_board_InitHardware = m74_9_boardInitHardware;
	custom_board_DefaultConfiguration = m74_9_boardDefaultConfiguration;
	custom_board_ConfigOverrides = m74_9_boardConfigOverrides;
}

