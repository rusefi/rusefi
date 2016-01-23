
#ifndef BOARDS_FT_H_
#define BOARDS_FT_H_

#define ADC_LOGIC_TPS 0
#define ADC_LOGIC_AFR 0
#define ADC_LOGIC_MAF 0
#define ADC_LOGIC_MAP 0
#define ADC_CHANNEL_VREF 0
#define ADC_CHANNEL_VBATT 0
#define ADC_LOGIC_INTAKE_AIR 0
#define ADC_LOGIC_COOLANT 0

#define CONSOLE_PORT (&SD1)
#define TS_SIMULATOR_PORT (&SD2)

/**
 * This implementation writes to both windows console and console port
 */
#define EFI_CONSOLE_UART_DEVICE (&testStream)

int getAdcValue(const char *msg, int channel);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BOARDS_FT_H_ */
