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
    { Gpio::A0, EFI_ADC_0, BIT(0) | BIT(1) },
    { Gpio::A1, EFI_ADC_1, BIT(0) | BIT(1) },
    { Gpio::A2, EFI_ADC_2, BIT(0) | BIT(1) },
    { Gpio::A3, EFI_ADC_3, BIT(0) | BIT(1) },
    { Gpio::A4, EFI_ADC_4, BIT(0) | BIT(1) },
    { Gpio::A5, EFI_ADC_5, BIT(0) | BIT(1) },
    { Gpio::A6, EFI_ADC_6, BIT(0) | BIT(1) },
    { Gpio::A7, EFI_ADC_7, BIT(0) | BIT(1) },
    { Gpio::B0, EFI_ADC_8, BIT(0) | BIT(1) },
    { Gpio::B1, EFI_ADC_9, BIT(0) | BIT(1) },
    { Gpio::C0, EFI_ADC_10, BIT(0) | BIT(1) },
    { Gpio::C1, EFI_ADC_11, BIT(0) | BIT(1) },
    { Gpio::C2, EFI_ADC_12, BIT(0) | BIT(1) },
    { Gpio::C3, EFI_ADC_13, BIT(0) | BIT(1) },
    { Gpio::C4, EFI_ADC_14, BIT(0) | BIT(1) },
    { Gpio::C5, EFI_ADC_15, BIT(0) | BIT(1) },
    /* TODO: add ADC3 channels */
};

brain_pin_e getAdcChannelBrainPin(const char *msg, adc_channel_e hwChannel) {
    static_assert(EFI_ADC_NONE == ADC_CHANNEL_NONE);

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
