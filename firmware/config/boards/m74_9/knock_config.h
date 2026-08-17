#pragma once

/* m74_9 (AT32F435) software knock.
 *
 * The knock sensor input (connector AA3, "Knock sensor +", with AB3 =
 * "Knock sensor ground") goes through the onboard SGM321/LM321 conditioner
 * (op-amp, AC-coupled) to PA0 = ADC1 IN0. The pin idles at ~1.76 V (bias
 * divider at the ADC input). Verified on the bench with the knocktest/
 * knockpin diagnostics.
 *
 * ADC1 is shared with the slow sampling (EFI_SLOW_ADC == KNOCK_ADC == ADCD1):
 * the driver is started once (portInitAdc), the slow background chain is
 * aborted and resumed around each knock window (onStartKnockSampling steals
 * via adcStopConversionI, knockCompletionCallback in stm32_adc_v2.cpp resumes
 * the chain). ADC3 stays with EFI_ADC3_SLOW (CLT/IAT live on the ADC3-only
 * F-port pins PF5/PF6) and has no other users.
 *
 * Timing: ADCCLK = HCLK/16/adcdiv = 5.94 MHz with adcdiv=1 (mcuconf
 * STM32_ADC_ADCPRE = DIV4, see the mcuconf.h comment). ADC_SAMPLE_56 programs
 * the Artery SPT bits to 24.5 cycles, one conversion = 24.5 + 12.5 = 37
 * cycles. VERIFY the real rate with the 'knocktest' console command and
 * adjust KNOCK_SAMPLE_RATE if it drifts. */
#define KNOCK_ADC ADCD1
#define KNOCK_ADC_CH1 ADC_CHANNEL_IN0
#define KNOCK_PIN_CH1 Gpio::A0

#define KNOCK_SAMPLE_TIME ADC_SAMPLE_56
#define KNOCK_SAMPLE_RATE (5940000 / (24 + 13))
