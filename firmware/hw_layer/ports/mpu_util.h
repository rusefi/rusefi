#pragma once

#include "rusefi_types.h"

#include "port_mpu_util.h"

#ifdef __cplusplus

// Base MCU
void baseMCUInit(void);
void jump_to_bootloader();
#if EFI_USE_OPENBLT
void jump_to_openblt();
#endif
void causeHardFault();
bool allowFlashWhileRunning();

// 300 ms is our default timeout (we use 'int' for compatibility with addConsoleActionI())
void startWatchdog(int timeoutMs = 300);
void setWatchdogResetPeriod(int resetMs);
void tryResetWatchdog();

bool ramReadProbe(volatile const char *read_address);
#if defined(STM32F4)
bool isStm32F42x();
#endif // STM32F4

// ADC
#if HAL_USE_ADC

adc_channel_e getAdcChannel(brain_pin_e pin);
brain_pin_e getAdcChannelBrainPin(const char *msg, adc_channel_e hwChannel);

// deprecated - migrate to 'getAdcChannelBrainPin'
ioportid_t getAdcChannelPort(const char *msg, adc_channel_e hwChannel);
// deprecated - migrate to 'getAdcChannelBrainPin'
int getAdcChannelPin(adc_channel_e hwChannel);

void portInitAdc();
float getMcuTemperature();
// Convert all slow ADC inputs.  Returns true if the conversion succeeded, false if a failure occured.
bool readSlowAnalogInputs(adcsample_t* convertedSamples);
#endif

// CAN bus
#if HAL_USE_CAN
bool isValidCanTxPin(brain_pin_e pin);
bool isValidCanRxPin(brain_pin_e pin);
CANDriver* detectCanDevice(brain_pin_e pinRx, brain_pin_e pinTx);
#endif // HAL_USE_CAN

// Serial
#if EFI_AUX_SERIAL
bool isValidSerialTxPin(brain_pin_e pin);
bool isValidSerialRxPin(brain_pin_e pin);
#endif //EFI_AUX_SERIAL

// SPI
#if HAL_USE_SPI
void initSpiModule(SPIDriver *driver, brain_pin_e sck, brain_pin_e miso,
		brain_pin_e mosi,
		int sckMode,
		int mosiMode,
		int misoMode);

void initSpiCs(SPIConfig *spiConfig, brain_pin_e csPin);
void turnOnSpi(spi_device_e device);
#endif // HAL_USE_SPI

#if HAL_USE_ICU
bool getIcuParams(brain_pin_e hwPin, iomode_t *af_ptr, ICUDriver ** icu_ptr, icuchannel_t *channel_ptr, uint32_t *clock_ptr);
#endif

// MMC Card
#if HAL_USE_MMC_SPI
// HS = max 50MHz SPI
extern SPIConfig mmc_hs_spicfg;
// LS = max 25MHz SPI
extern SPIConfig mmc_ls_spicfg;
#endif

// Hardware PWM
struct hardware_pwm {
	static hardware_pwm* tryInitPin(const char* msg, brain_pin_e pin, float frequencyHz, float duty);
	virtual void setDuty(float duty) = 0;
};

// Brownout Reset
typedef enum {
	BOR_Result_Ok = 0x00,
	BOR_Result_Error
} BOR_Result_t;

BOR_Level_t BOR_Get(void);
BOR_Result_t BOR_Set(BOR_Level_t BORValue);

#ifdef AT32F4XX
int at32GetMcuType(uint32_t id, const char **pn, const char **package, uint32_t *flashSize);
int at32GetRamSizeKb(void);
#endif

extern "C"
{
#endif /* __cplusplus */

// these need to be declared with C linkage - they're called from C and asm files
void DebugMonitorVector(void);
void UsageFaultVector(void);
void BusFaultVector(void);
void HardFaultVector(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
