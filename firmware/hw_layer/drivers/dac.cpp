#include "dac.h"

#include <rusefi/math.h>

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

void Dac::Start(DACConfig& config)
{
    dacStart(m_driver, &config);
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

static Dac dac(DACD1);

void initDac() {
    dac.Start(dacConfig);
    for (size_t i = 0;i<DAC_OUTPUT_COUNT;i++) {
        Gpio pin = engineConfiguration->dacOutputPins[i];
        if (isBrainPinValid(pin)) {
            // setting up the output pin as analog as suggested by the Reference Manual.
            efiSetPadMode("dac", pin, PAL_MODE_INPUT_ANALOG);
        }
    }
}

void setDacVoltage(int channel, float voltage) {
    dac.SetVoltage(channel, voltage);
}

#endif // EFI_DAC
