
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

#define DELEGATE (&SD1)

#define EFI_CONSOLE_UART_DEVICE (&testStream)

//float getVoltageDivided(int);
int getAdcValue(int channel);
void initFakeBoard(void);

#endif /* BOARDS_FT_H_ */
