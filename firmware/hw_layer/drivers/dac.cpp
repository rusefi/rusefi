#include "dac.h"

#include <rusefi/rusefi_math.h>

#if EFI_DAC

static DACConfig dacConfig = {
  .init         = 4095U,    /* full VCC */
  .datamode     = DAC_DHRM_12BIT_RIGHT,
  .cr           = 0
};

Dac::Dac(DACDriver& driver)
    : m_driver(&driver)
{
}

void Dac::Start(DACConfig& p_config)
{
    dacStart(m_driver, &p_config);
}

void Dac::SetVoltage(int channel, float voltage) {
    voltage = clampF(0, voltage, engineConfiguration->adcVcc);
    m_voltageFloat[channel] = voltage;

    dacPutChannelX(m_driver, channel, voltage / engineConfiguration->adcVcc * ((1 << 12) - 1));
}

float Dac::GetLastVoltage(int channel)
{
    return m_voltageFloat[channel];
}

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
        if (isBrainPinValid(pin)) {
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
    dacs[channel].SetVoltage(0, voltage);
}

#endif // EFI_DAC
