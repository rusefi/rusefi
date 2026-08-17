#pragma once

/* m74_9 (AT32F435) software knock.
 *
 * The knock sensor input (connector AA3, "Knock sensor +", with AB3 =
 * "Knock sensor ground") goes through the onboard conditioner to an MCU ADC
 * pin. The exact pin is not recorded in the board YAML; PF7 (ADC3 IN5) is
 * assumed following the hellen boards (hellen_knock_config.h uses the same
 * ADC3 IN5 / PF7). VERIFY on the bench: in the console 'adc_report' the knock
 * channel idles at ~1.65 V (open inputs read ~3.28 V); tapping the knock
 * sensor moves exactly one channel. If another channel reacts, change
 * KNOCK_ADC_CH1 / KNOCK_PIN_CH1.
 *
 * ADC3 is shared with EFI_ADC3_SLOW (CLT/IAT live on the ADC3-only F-port
 * pins PF5/PF6) - see readSlowAnalogInputs() in stm32_adc_v2.cpp for the
 * mutual exclusion contract. */
#define KNOCK_ADC ADCD3
#define KNOCK_ADC_CH1 ADC_CHANNEL_IN5
#define KNOCK_PIN_CH1 Gpio::F7

/* AT32F435 ADC timing: ADCCLK = HCLK/4 = 72 MHz (mcuconf STM32_ADC_ADCPRE =
 * ADC_CCR_ADCPRE_DIV6 -> Artery ADCDIV = HCLK/4, see docs/report.md).
 * ADC_SAMPLE_480 programs the Artery SPT bits to 239.5 sample cycles, so one
 * conversion takes 239.5 + 12.5 = 252 ADCCLK cycles. */
#define KNOCK_SAMPLE_TIME ADC_SAMPLE_480
#define KNOCK_SAMPLE_RATE (72000000 / (239 + 13))
