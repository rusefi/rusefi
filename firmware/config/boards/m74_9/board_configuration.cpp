
#include "pch.h"
#include "smart_gpio.h"
#include "drivers/gpio/l9779.h"

void setBoardConfigOverrides() {
	// PB14 is error LED, configured in board.mk
	// blue, LD2
	engineConfiguration->communicationLedPin = Gpio::B7;
	// green, LD1
	engineConfiguration->runningLedPin = Gpio::B0;

	// Board only has 3 LEDs
	engineConfiguration->warningLedPin = Gpio::Unassigned;
}

void preHalInit() {
	// leftovers from Nucleo board...
	efiSetPadMode("Ethernet",  Gpio::A1, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::A2, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::A7, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("Ethernet", Gpio::B13, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("Ethernet",  Gpio::C1, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::C4, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::C5, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("Ethernet", Gpio::G11, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet", Gpio::G13, PAL_MODE_ALTERNATE(0xb));
}

static struct l9779_config l9779_cfg = {
	.spi_bus = &SPID1,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		.ssport = NULL,
		.sspad = 0,
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
