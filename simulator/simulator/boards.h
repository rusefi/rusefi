
#pragma once

#define ADC_LOGIC_TPS 0
#define ADC_LOGIC_AFR 0
#define ADC_LOGIC_MAF 0
#define ADC_LOGIC_MAP 0
#define ADC_CHANNEL_VREF 0
#define ADC_CHANNEL_VBATT 0
#define ADC_LOGIC_INTAKE_AIR 0
#define ADC_LOGIC_COOLANT 0

int getAdcValue(const char *msg, int channel);
#define getSlowAdcCounter() 0
#define waitForSlowAdc(x) {}


// mock SPI
#define SPIDriver void
typedef uint16_t adcsample_t;
#define spiStart(x, y) {}
#define spiStop(x) {}
#define spiSelect(x) {}
#define spiSelectI(x) {}
#define spiPolledExchange(x, y) {}
#define spiSend(x, y, z) {}
#define spiReceive(x, y, z) {}
#define spiExchange(x,y,w,z) {}
#define spiExchangeI(x,y,w,z) {}
#define spiStartExchangeI(x,y,w,z) {}
#define spiStartExchange(x,y,w,z) {}
#define spiUnselect(x) {}
#define spiUnselectI(x) {}
#define spiAcquireBus(x) {}
#define spiReleaseBus(x) {}
// end of mock SPI

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


#ifdef __cplusplus
}
#endif /* __cplusplus */

