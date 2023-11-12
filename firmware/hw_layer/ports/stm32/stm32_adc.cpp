/**
 * @file	stm32_common_adc.cpp
 * @brief	Low level common STM32 code
 *
 * @date Mar 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if HAL_USE_ADC

// ADC_CHANNEL_IN0 // PA0
// ADC_CHANNEL_IN1 // PA1
// ADC_CHANNEL_IN2 // PA2
// ADC_CHANNEL_IN3 // PA3
// ADC_CHANNEL_IN4 // PA4
// ADC_CHANNEL_IN5 // PA5 - this is also TIM2_CH1
// ADC_CHANNEL_IN6 // PA6
// ADC_CHANNEL_IN7 // PA7
// ADC_CHANNEL_IN8 // PB0
// ADC_CHANNEL_IN9 // PB1
// ADC_CHANNEL_IN10 // PC0
// ADC_CHANNEL_IN11 // PC1
// ADC_CHANNEL_IN12 // PC2
// ADC_CHANNEL_IN13 // PC3
// ADC_CHANNEL_IN14 // PC4
// ADC_CHANNEL_IN15 // PC5

brain_pin_e getAdcChannelBrainPin(const char *msg, adc_channel_e hwChannel) {
    static_assert(EFI_ADC_NONE == ADC_CHANNEL_NONE);

    // todo: replace this with an array :)
    switch (hwChannel) {
    case EFI_ADC_0:
        return Gpio::A0;
    case EFI_ADC_1:
        return Gpio::A1;
    case EFI_ADC_2:
        return Gpio::A2;
    case EFI_ADC_3:
        return Gpio::A3;
    case EFI_ADC_4:
        return Gpio::A4;
    case EFI_ADC_5:
        return Gpio::A5;
    case EFI_ADC_6:
        return Gpio::A6;
    case EFI_ADC_7:
        return Gpio::A7;
    case EFI_ADC_8:
        return Gpio::B0;
    case EFI_ADC_9:
        return Gpio::B1;
    case EFI_ADC_10:
        return Gpio::C0;
    case EFI_ADC_11:
        return Gpio::C1;
    case EFI_ADC_12:
        return Gpio::C2;
    case EFI_ADC_13:
        return Gpio::C3;
    case EFI_ADC_14:
        return Gpio::C4;
    case EFI_ADC_15:
        return Gpio::C5;
    default:
/* todo: what is upper range ADC is used while lower range ADC is not used? how do we still mark pin used?
external muxes for internal ADC #3350
        firmwareError(ObdCode::CUSTOM_ERR_ADC_UNKNOWN_CHANNEL, "Unknown hw channel %d [%s]", hwChannel, msg);
*/
        return Gpio::Invalid;
    }
}

adc_channel_e getAdcChannel(brain_pin_e pin) {
    switch (pin) {
    case Gpio::A0:
        return EFI_ADC_0;
    case Gpio::A1:
        return EFI_ADC_1;
    case Gpio::A2:
        return EFI_ADC_2;
    case Gpio::A3:
        return EFI_ADC_3;
    case Gpio::A4:
        return EFI_ADC_4;
    case Gpio::A5:
        return EFI_ADC_5;
    case Gpio::A6:
        return EFI_ADC_6;
    case Gpio::A7:
        return EFI_ADC_7;
    case Gpio::B0:
        return EFI_ADC_8;
    case Gpio::B1:
        return EFI_ADC_9;
    case Gpio::C0:
        return EFI_ADC_10;
    case Gpio::C1:
        return EFI_ADC_11;
    case Gpio::C2:
        return EFI_ADC_12;
    case Gpio::C3:
        return EFI_ADC_13;
    case Gpio::C4:
        return EFI_ADC_14;
    case Gpio::C5:
        return EFI_ADC_15;
    case Gpio::Unassigned:
        return EFI_ADC_NONE;
    default:
        criticalError("getAdcChannel %d", pin);
        return EFI_ADC_ERROR;
    }
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
