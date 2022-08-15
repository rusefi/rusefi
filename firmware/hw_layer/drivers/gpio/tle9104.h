#pragma once

#if BOARD_TLE9104_COUNT > 0

struct tle9104_config {
	SPIDriver		*spi_bus;
	SPIConfig	spi_config;

	struct {
		ioportid_t		port;
		uint_fast8_t	pad;
	} direct_io[4];

	Gpio resn;
	Gpio en;
};

void tle9104_add(Gpio base, int index, const tle9104_config* cfg);

// This example config worked on the bench!
// tle9104_config tle9104_cfg = {
// 	.spi_bus = NULL,
// 	.spi_config = {
// 		.circular = false,
// 		.end_cb = NULL,
// 		.ssport = NULL,
// 		.sspad = 0,
// 		.cr1 =
// 			SPI_CR1_16BIT_MODE |
// 			SPI_CR1_SSM |
// 			SPI_CR1_SSI |
// 			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	// div = 16
// 			SPI_CR1_MSTR |
// 			SPI_CR1_CPHA |
// 			0,
// 		.cr2 = SPI_CR2_16BIT_MODE
// 	},
// 	.direct_io = {
// 		{ GPIOB, 12 },
// 		{ GPIOB, 13 },
// 		{ GPIOB, 14 },
// 		{ GPIOB, 15 }
// 	},
// 	.resn = Gpio::A3,
// 	.en   = Gpio::C9
// };

#endif // BOARD_TLE9104_COUNT > 0
