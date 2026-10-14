#include "dac.h"

#include <rusefi/rusefi_math.h>

#if EFI_DAC || EFI_UNIT_TEST

static constexpr DACConfig makeDacConfig() {
    DACConfig result{};
    result.init = 4095U; /* full VCC */
    result.datamode = DAC_DHRM_12BIT_RIGHT;
    result.cr = 0;
    return result;
}

static DACConfig dacConfig = makeDacConfig();

Dac::Dac(DACDriver& driver)
    : m_driver(&driver)
{
}

void Dac::Start(DACConfig& p_config)
{
    if (!m_isStarted) {
        dacStart(m_driver, &p_config);
        m_isStarted = true;
    }
}

void Dac::SetConfigured(bool configured)
{
    m_isConfigured = configured;
}

void Dac::SetVoltage(int channel, float voltage) {
    // [tag:unassign_outputs] A retained Lua script must not write to a DAC that
    // was left unconfigured after clearing its pins and rebooting.
    if (channel < 0 || channel >= 2 || !m_isConfigured || !m_isStarted) {
        return;
    }

    voltage = clampF(0, voltage, engineConfiguration->adcVcc);
    m_voltageFloat[channel] = voltage;

    dacPutChannelX(m_driver, channel, voltage / engineConfiguration->adcVcc * ((1 << 12) - 1));
}

float Dac::GetLastVoltage(int channel)
{
    return m_voltageFloat[channel];
}

#if EFI_UNIT_TEST
void Dac::ResetForUnitTest()
{
	m_isStarted = false;
	m_isConfigured = false;
	m_voltageFloat[0] = 0;
	m_voltageFloat[1] = 0;
}
#endif

static Dac dacs[] {
    /** ChibiOS magic: if not dual mode DAC1 CH1 driver identifier.*/
    Dac(DACD1),
    /** ChibiOS magic: if not dual mode DAC1 CH2 driver identifier.*/
    Dac(DACD2)
};

void initDac() {
    bool dacIsUsed = false;
    for (size_t i = 0;i<DAC_OUTPUT_COUNT;i++) {
        Gpio pin = engineConfiguration->dacOutputPins[i];
        bool isConfigured = isBrainPinValid(pin);
        dacs[i].SetConfigured(isConfigured);
        if (isConfigured) {
            // setting up the output pin as analog as suggested by the Reference Manual.
            efiSetPadMode("dac", pin, PAL_MODE_INPUT_ANALOG);
            dacIsUsed = true;
        }
    }
    if (dacIsUsed) {
        // DAC peripheral changes associated pin behaviour regardless of actual pin mode
        dacs[0].Start(dacConfig);
        dacs[1].Start(dacConfig);
    }
}

void setDacVoltage(int channel, float voltage) {
    if (channel < 0 || channel >= DAC_OUTPUT_COUNT) {
        return;
    }

    dacs[channel].SetVoltage(0, voltage);
}

#if EFI_UNIT_TEST
void resetDacForUnitTest()
{
	for (Dac& dac : dacs) {
		dac.ResetForUnitTest();
	}
}
#endif

#endif // EFI_DAC || EFI_UNIT_TEST
