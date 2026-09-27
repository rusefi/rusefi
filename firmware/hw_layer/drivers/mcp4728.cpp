/**
 * MCP4728 support for five chips (20 outputs) and one-time address programming.
 */
#include "pch.h"

#include "mcp4728.h"

#if LUA_I2C_DAC

#include <cmath>

bool Mcp4728::initPins(brain_pin_e scl, brain_pin_e sda, brain_pin_e ldac) {
	if (m_initialized) {
		// Lua scripts can restart without releasing/reclaiming active pins.
		return m_scl == scl && m_sda == sda && m_ldac == ldac;
	}

	auto onChip = [](brain_pin_e pin) { return pin >= Gpio::A0 && pin <= BRAIN_PIN_ONCHIP_LAST; };
	if (!onChip(scl) || !onChip(sda) || !onChip(ldac)
		|| scl == sda || scl == ldac || sda == ldac) {
		return false;
	}

#if EFI_PROD_CODE
	if (getPinFunction(scl) || getPinFunction(sda) || getPinFunction(ldac)) {
		return false;
	}
#endif

	if (!BitbangI2c::init(scl, sda, I2C_SPEED_100K)) {
		return false;
	}

	m_ldac = ldac;
#if EFI_PROD_CODE
	efiSetPadMode("MCP4728 LDAC", ldac, PAL_MODE_INPUT);
#endif
	m_initialized = true;
	return true;
}

bool Mcp4728::setValue(int channel, int value) {
	if (!m_initialized || channel < 1 || channel > ChannelCount || value < 0 || value > MaxValue) {
		return false;
	}

	const int index = channel - 1;
	// Multi-write: VDD reference, normal power, gain 1, UDAC=0 (update immediately).
	// Unlike fast write, this also replaces any reference/gain restored from EEPROM.
	const uint8_t data[] = {
		static_cast<uint8_t>(0x40 | ((index % 4) << 1)),
		static_cast<uint8_t>(value >> 8),
		static_cast<uint8_t>(value),
	};
	return write(0x60 + index / 4, data, sizeof(data)) == MSG_OK;
}

bool Mcp4728::setVoltage(int channel, float voltage, float vcc) {
	if (!std::isfinite(voltage) || !std::isfinite(vcc) || vcc <= 0 || voltage < 0 || voltage > vcc) {
		return false;
	}

	// MCP4728 transfer function: Vout = VDD * code / 4096.
	const float code = voltage / vcc * 4096.0f;
	return setValue(channel, code >= MaxValue ? MaxValue : static_cast<int>(code));
}

bool Mcp4728::setChipValues(int address, const uint16_t (&values)[4]) {
	if (!m_initialized || address < 0 || address >= ChipCount) {
		return false;
	}

	uint8_t data[12];
	for (int i = 0; i < 4; i++) {
		if (values[i] > MaxValue) {
			return false;
		}
		data[3 * i] = 0x40 | (i << 1);
		data[3 * i + 1] = values[i] >> 8;
		data[3 * i + 2] = values[i];
	}
	return write(0x60 + address, data, sizeof(data)) == MSG_OK;
}

void Mcp4728::waitQuarterBit() {
#if EFI_PROD_CODE
	// Stay below 100 kHz across MCU clock speeds, without reserving a timer.
	chSysPolledDelayX(US2RTC(SystemCoreClock, 3));
#endif
}

void Mcp4728::setLdac(bool high) {
#if EFI_PROD_CODE
	palWritePad(getHwPort("MCP4728 LDAC", m_ldac), getHwPin("MCP4728 LDAC", m_ldac), high);
#else
	UNUSED(high);
#endif
}

void Mcp4728::waitForEeprom() {
#if EFI_PROD_CODE
	chThdSleepMilliseconds(100);
#endif
}

bool Mcp4728::programAddress(int address) {
	if (!m_initialized || address < 0 || address > 7 || lock() != MSG_OK) {
		return false;
	}

	setLdac(true);
#if EFI_PROD_CODE
	efiSetPadModeWithoutOwnershipAcquisition("MCP4728 LDAC", m_ldac, PAL_MODE_OUTPUT_PUSHPULL);
#endif
	start();
	bool ack = writeByte(0x60 << 1); // factory address 0, write
	if (ack) {
		const uint8_t command = 0x61; // current address 0
		for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
			sendBit(command & mask);
		}
		// Datasheet figure 5-11: LDAC falls after clock 8, before ACK clock 9.
		setLdac(false);
		sda_high();
		ack = !readBit();
	}
	if (ack) {
		ack = writeByte(0x62 | (address << 2));
	}
	if (ack) {
		ack = writeByte(0x63 | (address << 2));
	}
	stop();

	// Keep other writers off this bus until the EEPROM cycle finishes.
	if (ack) {
		waitForEeprom();
	}
#if EFI_PROD_CODE
	efiSetPadModeWithoutOwnershipAcquisition("MCP4728 LDAC", m_ldac, PAL_MODE_INPUT);
#endif
	unlock();
	return ack;
}

static Mcp4728 luaI2cDac;

Mcp4728& getLuaI2cDac() {
	return luaI2cDac;
}

void initI2cDacConsole() {
	addConsoleActionI("at_set_dac_addr", [](int address) {
		if (address < 0 || address > 7) {
			efiPrintf("DAC address must be 0..7");
			return;
		}
		efiPrintf("MCP4728 address %d: %s", address,
			getLuaI2cDac().programAddress(address) ? "Success!" : "Error! Check initI2cDac, wiring and factory address 0");
	});
}

#endif // LUA_I2C_DAC
