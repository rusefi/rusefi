
#include "pch.h"
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

/**
 * @brief   Board-specific configuration defaults.
 * @todo    Add your board-specific code, if any.
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
	engineConfiguration->spi1MosiMode = PO_DEFAULT;
	engineConfiguration->spi1misoPin = Gpio::E14;
	engineConfiguration->spi1MisoMode = PO_DEFAULT;
	engineConfiguration->spi1sckPin = Gpio::E13;
	engineConfiguration->spi1SckMode = PO_DEFAULT;
}

void setBoardConfigOverrides() {
	//CAN 1 bus overwrites
	engineConfiguration->canRxPin = Gpio::G0;
	engineConfiguration->canTxPin = Gpio::G1;
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
		/* ignition pre-dirvers */
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
