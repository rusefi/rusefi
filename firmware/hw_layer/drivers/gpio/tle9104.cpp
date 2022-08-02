#include "pch.h"
#include "gpio/gpio_ext.h"
#include "gpio/tle9104.h"

#if BOARD_TLE9104_COUNT > 0

#define TLE9104_REG_CTRL 0x00
#define TLE9104_REG_CFG 0x01
#define TLE9104_REG_OFF_DIAG_CFG 0x02
#define TLE9104_REG_ON_DIAG_CFG 0x03
#define TLE9104_REG_DIAG_OUT_1_2_ON 0x04
#define TLE9104_REG_DIAG_OUT_3_4_ON 0x05
#define TLE9104_REG_DIAG_OFF 0x06
#define TLE9104_REG_GLOBAL_STATUS 0x07
#define TLE9104_REG_ICVID 0x08

struct Tle9104 : public GpioChip {
	int init() override;

	int writePad(size_t pin, int value) override;
	brain_pin_diag_e getDiag(size_t pin) override;


	uint16_t readWrite(uint8_t addr, uint8_t data);
	uint16_t read(uint8_t addr);
	void write(uint8_t addr, uint8_t data);

	void updateDiagState();

	const tle9104_config* cfg;


	uint8_t diag_off;
	uint8_t diag_on12;
	uint8_t diag_on34;

	OutputPin m_en;
	OutputPin m_resn;
};

static bool parityBit(uint16_t val) {
	// (1 + number of bits set) mod 2 = parity bit
	int count = 1;

	while (val != 0) {
		if (val & 0x01) {
			count++;
		}

		val = val >> 1;
	}

	return (count % 2) == 1;
}

uint16_t Tle9104::readWrite(uint8_t addr, uint8_t data) {
	uint16_t tx = (addr << 8) + data;

	// set the parity bit appropriately
	tx |= parityBit(tx) ? (1 << 14) : 0;

	SPIDriver* spi = cfg->spi_bus;

	spiSelect(spi);
	uint16_t rx = spiPolledExchange(spi, tx);
	spiUnselect(spi);

	bool parityOk = parityBit(rx);

	// return data
	return rx;
}

uint16_t Tle9104::read(uint8_t addr) {
	// R/W bit is 0 for read
	readWrite(addr, 0);

	return readWrite(addr, 0);
}

void Tle9104::write(uint8_t addr, uint8_t data) {
	// R/W bit is 1 for write
	readWrite(0x80 | addr, data);
}

int Tle9104::init() {
	m_resn.initPin("TLE9104 RESN", cfg->resn);
	m_en.initPin("TLE9104 EN", cfg->en);

	// disable outputs
	m_en.setValue(false);

	for (int i = 0; i < 4; i++) {
		gpio_pin_markUsed(cfg->direct_io[i].port, cfg->direct_io[i].pad, "TLE9104 Direct IO");
		palSetPadMode(cfg->direct_io[i].port, cfg->direct_io[i].pad, PAL_MODE_OUTPUT_PUSHPULL);

		// Ensure all outputs are off
		writePad(i, false);
	}

	// Reset the chip
	m_resn.setValue(false);
	chThdSleepMilliseconds(1);
	m_resn.setValue(true);
	chThdSleepMilliseconds(1);

	spiStart(cfg->spi_bus, &cfg->spi_config);

	// read ID register
	uint16_t id = read(TLE9104_REG_ICVID);

	// No chip detected if ID is wrong
	if ((id & 0xFF) != 0xB1) {
		return -1;
	}

	// disable comms watchdog, enable direct drive on all 4 channels
	// TODO: should we enable comms watchdog?
	write(TLE9104_REG_CFG, 0x0F);

	// clear any suprious diag states from startup: first call resets, second reads true state
	updateDiagState();
	updateDiagState();

	// set output enable, clear all other flags
	write(TLE9104_REG_GLOBAL_STATUS, 0x80);

	// Set hardware enable
	m_en.setValue(true);

	return 0;
}

int Tle9104::writePad(size_t pin, int value) {
	// Inverted since TLE9104 is active low (falling edge to turn on output)
	if (value) {
		palClearPad(cfg->direct_io[pin].port, cfg->direct_io[pin].pad);
	} else {
		palSetPad(cfg->direct_io[pin].port, cfg->direct_io[pin].pad);
	}

	return 0;
}

void Tle9104::updateDiagState() {
	spiStart(cfg->spi_bus, &cfg->spi_config);

	diag_on12 = read(TLE9104_REG_DIAG_OUT_1_2_ON);
	diag_on34 = read(TLE9104_REG_DIAG_OUT_3_4_ON);
	diag_off = read(TLE9104_REG_DIAG_OFF);

	// clear diag states
	write(TLE9104_REG_DIAG_OUT_1_2_ON, 0);
	write(TLE9104_REG_DIAG_OUT_3_4_ON, 0);
	write(TLE9104_REG_DIAG_OFF, 0);
}

brain_pin_diag_e Tle9104::getDiag(size_t pin) {
	int off_diag;
	int on_diag;

	switch (pin) {
		case 0:
			on_diag = diag_on12;
			off_diag = diag_off;
			break;
		case 1:
			on_diag = diag_on12 >> 3;
			off_diag = diag_off >> 2;
			break;
		case 2:
			on_diag = diag_on34;
			off_diag = diag_off >> 4;
			break;
		case 3:
			on_diag = diag_on34 >> 3;
			off_diag = diag_off >> 6;
			break;
		default:
			return PIN_INVALID;
	}

	// on diag has 3 bits
	on_diag = on_diag & 0x7;
	// of diag has 2 bits
	off_diag = off_diag & 0x3;

	int result = 0;

	// Decode on-state faults
	switch (on_diag) {
		case 2:
			result |= PIN_SHORT_TO_BAT;
			break;
		case 3:
			// overtemp and overcurrent
			result |= PIN_DRIVER_OVERTEMP;
			// falls through
		case 4:
			result |= PIN_OVERLOAD;
			break;
		case 5:
			result |= PIN_DRIVER_OVERTEMP;
			break;
	}

	// Decode off-state faults
	switch (off_diag) {
		case 2:
			result |= PIN_OPEN;
			break;
		case 3:
			result |= PIN_SHORT_TO_GND;
			break;
	}

	return (brain_pin_diag_e)result;
}

static Tle9104 chips[4];

void tle9104_add(Gpio base, int index, const tle9104_config* cfg) {
	Tle9104& chip = chips[index];

	chip.cfg = cfg;
	gpiochip_register(base, "TLE9104", chip, 4);
}

void updatetlediag() {
	chips[0].updateDiagState();
}

#endif // BOARD_TLE9104_COUNT > 0
