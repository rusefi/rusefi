/**
 * @file	stm32_common_adc.cpp
 * @brief	Low level common STM32 code
 *
 * @date Mar 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "efilib.h"

#if HAL_USE_ADC

static const struct {
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

brain_pin_e getAdcChannelBrainPin(const char *msg, adc_channel_e hwChannel) {
    static_assert(EFI_ADC_NONE == ADC_CHANNEL_NONE);

    /* Muxed adc inputs
     * TODO: move this magic to adcChannels[] table? */
    if (adcIsMuxedInput(hwChannel)) {
        return getAdcChannelBrainPin(msg, (adc_channel_e)(EFI_ADC_0 + (hwChannel - EFI_ADC_16)));
    }

    for (size_t idx = 0; idx < efi::size(adcChannels); idx++) {
        if (adcChannels[idx].ch == hwChannel) {
            return adcChannels[idx].pin;
        }
    }

    /* todo: what is upper range ADC is used while lower range ADC is not used? how do we still mark pin used?
     * external muxes for internal ADC #3350
     * firmwareError(ObdCode::CUSTOM_ERR_ADC_UNKNOWN_CHANNEL, "Unknown hw channel %d [%s]", hwChannel, msg);
     */
    (void)msg;

    return Gpio::Invalid;
}

bool adcIsMuxedInput(adc_channel_e hwChannel) {
#ifdef ADC_MUX_PIN
    return ((hwChannel >= EFI_ADC_16) && (hwChannel <= EFI_ADC_31));
#else
    return false;
#endif
}

adc_channel_e getAdcChannel(brain_pin_e pin) {
    if (pin == Gpio::Unassigned)
        return EFI_ADC_NONE;

    for (size_t idx = 0; idx < efi::size(adcChannels); idx++) {
        if (adcChannels[idx].pin == pin) {
            return adcChannels[idx].ch;
        }
    }

    criticalError("getAdcChannel %d", pin);
    return EFI_ADC_ERROR;
}

// Get ADC internal input index for given hwChannel
int getAdcInternalChannel(ADC_TypeDef *adc, adc_channel_e hwChannel)
{
    uint8_t mask = 0;

#if STM32_ADC_USE_ADC1
    if (adc == ADC1) {
        mask = BIT(0);
    }
#endif
#if STM32_ADC_USE_ADC2
    if (adc == ADC2) {
        mask = BIT(1);
    }
#endif
#if STM32_ADC_USE_ADC3
    if (adc == ADC3) {
        mask = BIT(2);
    }
#endif

    if (mask == 0) {
        // Unknown ADC instance
        return -1;
    }

    int hwIndex = 0;
    for (size_t idx = 0; idx < efi::size(adcChannels); idx++) {
        if (adcChannels[idx].ch == hwChannel) {
            return hwIndex;
        }
        if (adcChannels[idx].adc & mask) {
            hwIndex++;
        }
    }

    // Channel is not supported by this ADC
    return -1;

}

// deprecated - inline?
ioportid_t getAdcChannelPort(const char *msg, adc_channel_e hwChannel) {
    brain_pin_e brainPin = getAdcChannelBrainPin(msg, hwChannel);
    return getHwPort(msg, brainPin);
}

// deprecated - inline?
int getAdcChannelPin(adc_channel_e hwChannel) {
    brain_pin_e brainPin = getAdcChannelBrainPin("get_pin", hwChannel);
    return getHwPin("get_pin", brainPin);
}

#endif /* HAL_USE_ADC */
