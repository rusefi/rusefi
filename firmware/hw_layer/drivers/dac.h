// dac.h

#pragma once

#include <cstdint>

#include "pch.h"

#if EFI_DAC

class Dac
{
public:
    Dac(DACDriver& driver);

    void Start(DACConfig& config);
    void SetVoltage(int channel, float volage);
    float GetLastVoltage(int channel);

private:
    DACDriver* const m_driver;
    float m_voltageFloat[2];
};

void initDac();
void setDacVoltage(int channel, float voltage);

#endif
