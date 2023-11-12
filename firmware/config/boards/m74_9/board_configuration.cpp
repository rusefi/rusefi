
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

/**
 * @brief   Board-specific configuration defaults.
 * @todo    Add your board-specific code, if any.
 */
void setBoardDefaultConfiguration() {
	/* SPI1 is used for communication with L9779 */
	engineConfiguration->is_enabled_spi_1 = true;

	engineConfiguration->spi1mosiPin = Gpio::E15;
	engineConfiguration->spi1MosiMode = PO_DEFAULT;
	engineConfiguration->spi1misoPin = Gpio::E14;
	engineConfiguration->spi1MisoMode = PO_DEFAULT;
	engineConfiguration->spi1sckPin = Gpio::E13;
	engineConfiguration->spi1SckMode = PO_DEFAULT;
}

static struct l9779_config l9779_cfg = {
	.spi_bus = &SPID1,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		.ssport = GPIOE,
		.sspad = 11,
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
		[0] = {.port = NULL, .pad = 0},
		[1] = {.port = NULL, .pad = 0},
		[2] = {.port = NULL, .pad = 0},
		[3] = {.port = NULL, .pad = 0},
		/* IN1 .. IN7 */
		[4] = {.port = NULL, .pad = 0},
		[5] = {.port = NULL, .pad = 0},
		[6] = {.port = NULL, .pad = 0},
		[7] = {.port = NULL, .pad = 0},
		[8] = {.port = NULL, .pad = 0},
		[9] = {.port = NULL, .pad = 0},
		[10] = {.port = NULL, .pad = 0},
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
