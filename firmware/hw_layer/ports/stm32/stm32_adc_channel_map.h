#pragma once

#include "global.h"

static constexpr struct {
    brain_pin_e pin;
    adc_channel_e ch;
    uint8_t adc;    /* bitmask of ADC available on this pin */
} adcChannels[] = {
    { Gpio::A0,  EFI_ADC_0,  BIT(0) | BIT(1) | BIT(2) },
    { Gpio::A1,  EFI_ADC_1,  BIT(0) | BIT(1) | BIT(2) },
    { Gpio::A2,  EFI_ADC_2,  BIT(0) | BIT(1) | BIT(2) },
    { Gpio::A3,  EFI_ADC_3,  BIT(0) | BIT(1) | BIT(2) },
    { Gpio::A4,  EFI_ADC_4,  BIT(0) | BIT(1)          },
    { Gpio::F6,  EFI_ADC_32,                   BIT(2) }, //ADC3 only
    { Gpio::A5,  EFI_ADC_5,  BIT(0) | BIT(1)          },
    { Gpio::F7,  EFI_ADC_33,                   BIT(2) }, //ADC3 only
    { Gpio::A6,  EFI_ADC_6,  BIT(0) | BIT(1)          },
    { Gpio::F8,  EFI_ADC_34,                   BIT(2) }, //ADC3 only
    { Gpio::A7,  EFI_ADC_7,  BIT(0) | BIT(1)          },
    { Gpio::F9,  EFI_ADC_35,                   BIT(2) }, //ADC3 only
    { Gpio::B0,  EFI_ADC_8,  BIT(0) | BIT(1)          },
    { Gpio::F10, EFI_ADC_36,                   BIT(2) }, //ADC3 only
    { Gpio::B1,  EFI_ADC_9,  BIT(0) | BIT(1)          },
    { Gpio::F3,  EFI_ADC_37,                   BIT(2) }, //ADC3 only
    { Gpio::C0,  EFI_ADC_10, BIT(0) | BIT(1) | BIT(2) },
    { Gpio::C1,  EFI_ADC_11, BIT(0) | BIT(1) | BIT(2) },
    { Gpio::C2,  EFI_ADC_12, BIT(0) | BIT(1) | BIT(2) },
    { Gpio::C3,  EFI_ADC_13, BIT(0) | BIT(1) | BIT(2) },
    { Gpio::C4,  EFI_ADC_14, BIT(0) | BIT(1)          },
    { Gpio::F4,  EFI_ADC_38,                   BIT(2) }, //ADC3 only
    { Gpio::C5,  EFI_ADC_15, BIT(0) | BIT(1)          },
    { Gpio::F5,  EFI_ADC_39,                   BIT(2) }, //ADC3 only
    /* TODO: add ADC3 channels */
};

// mask identifies ADC1/ADC2/ADC3 as bits 0/1/2 respectively.
inline int getStm32AdcInternalChannel(uint8_t mask, adc_channel_e channel) {
    if (mask == 0) {
        return -1;
    }
    int hwIndex = 0;
    for (const auto& entry : adcChannels) {
        if (entry.ch == channel) {
            return (entry.adc & mask) ? hwIndex : -1;
        }
        if (entry.adc & mask) {
            hwIndex++;
        }
    }
    return -1;
}
