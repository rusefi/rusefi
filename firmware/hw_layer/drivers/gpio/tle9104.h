#pragma once

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
