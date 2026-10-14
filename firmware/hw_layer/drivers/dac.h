// dac.h

#pragma once

#include <cstdint>

#include "pch.h"

#if EFI_UNIT_TEST
struct DACDriver {
	int startCount = 0;
	int putCount = 0;
	int lastChannel = -1;
	unsigned lastValue = 0;
};

struct DACConfig {
	unsigned init;
	int datamode;
	int cr;
};

constexpr int DAC_DHRM_12BIT_RIGHT = 0;
constexpr int PAL_MODE_INPUT_ANALOG = 0;
extern DACDriver unitTestDacDriver1;
extern DACDriver unitTestDacDriver2;
#define DACD1 unitTestDacDriver1
#define DACD2 unitTestDacDriver2

void dacStart(DACDriver* driver, DACConfig* config);
void dacPutChannelX(DACDriver* driver, int channel, unsigned value);
#endif

#if EFI_DAC || EFI_UNIT_TEST

class Dac
{
public:
    Dac(DACDriver& driver);

    void Start(DACConfig& config);
    void SetConfigured(bool configured);
    void SetVoltage(int channel, float voltage);
    float GetLastVoltage(int channel);
#if EFI_UNIT_TEST
    void ResetForUnitTest();
#endif

private:
    DACDriver* const m_driver;
    float m_voltageFloat[2];
    bool m_isStarted = false;
    bool m_isConfigured = false;
};

void initDac();
void setDacVoltage(int channel, float voltage);
#if EFI_UNIT_TEST
void resetDacForUnitTest();
#endif

#endif
