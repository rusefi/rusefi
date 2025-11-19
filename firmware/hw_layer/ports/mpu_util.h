#pragma once

#include "rusefi_types.h"

#include "port_mpu_util.h"
#include "mpu_watchdog.h"

#ifdef __cplusplus

// Base MCU
void baseMCUInit();
void jump_to_bootloader();
#if EFI_USE_OPENBLT
void jump_to_openblt();
#endif
void causeHardFault();

// If mcu can erase/write part of its internal memory without stalling CPU
bool mcuCanFlashWhileRunning();

#if defined(STM32F4)
bool isStm32F42x();
#endif // STM32F4

// ADC
#if HAL_USE_ADC

adc_channel_e getAdcChannel(brain_pin_e pin);
brain_pin_e getAdcChannelBrainPin(const char *msg, adc_channel_e hwChannel);
bool adcIsMuxedInput(adc_channel_e hwChannel);
adc_channel_e adcMuxedGetParent(adc_channel_e hwChannel);
int getAdcInternalChannel(ADC_TypeDef *adc, adc_channel_e hwChannel);
adc_channel_e getHwChannelForAdcInput(ADC_TypeDef *adc, size_t hwIndex);

// deprecated - migrate to 'getAdcChannelBrainPin'
ioportid_t getAdcChannelPort(const char *msg, adc_channel_e hwChannel);
// deprecated - migrate to 'getAdcChannelBrainPin'
int getAdcChannelPin(adc_channel_e hwChannel);

void portInitAdc();
float getMcuTemperature();
float getMcuVrefVoltage();
// Convert all slow ADC inputs.  Returns true if the conversion succeeded, false if a failure occured.
bool readSlowAnalogInputs(adcsample_t* convertedSamples);
#endif

// CAN bus
#if HAL_USE_CAN
bool isValidCanTxPin(brain_pin_e pin);
bool isValidCanRxPin(brain_pin_e pin);
CANDriver* detectCanDevice(brain_pin_e pinRx, brain_pin_e pinTx);
void canHwInfo(CANDriver* cand);
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

void initSpiCsNoOccupy(SPIConfig *spiConfig, brain_pin_e csPin);
void initSpiCs(SPIConfig *spiConfig, brain_pin_e csPin);
void turnOnSpi(spi_device_e device);
int spiGetBaseClock(SPIDriver *spip);
int spiCalcClockDiv(SPIDriver *spip, SPIConfig *spiConfig, unsigned int clk);
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

// Reset Cause
typedef enum {
	Reset_Cause_Unknown = 0,
	Reset_Cause_IWatchdog,		// Independent hardware watchdog (we use it)
	Reset_Cause_WWatchdog,		// Window watchdog
	Reset_Cause_Soft_Reset,		// NVIC_SystemReset or by debugger
	Reset_Cause_NRST_Pin,		// Reset from NRST pin
	Reset_Cause_POR,			// POR/PDR reset
	Reset_Cause_Illegal_Mode,	// Reset after illegal Stop, Standby or Shutdown mode entry
	Reset_Cause_BOR,			// BOR reset
	Reset_Cause_Firewall,		// Firewall reset
	Reset_Cause_Option_Byte,	// Option byte load reset
} Reset_Cause_t;

Reset_Cause_t getMCUResetCause();
const char *getMCUResetCause(Reset_Cause_t cause);

#ifdef AT32F4XX
int at32GetMcuType(uint32_t id, const char **pn, const char **package, uint32_t *flashSize);
int at32GetRamSizeKb(void);
#endif

void assertInterruptPriority(const char* func, uint8_t expectedPrio);

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

// search:openblt_version
// ascii 'BL04' in reverse LBS byte order
#define BLT_CURRENT_VERSION 0x34304C42
#define BLT_BIN_VERSION_ADDR              ((uint32_t)0x08000024U)       /*! 3rd reserved DWORD in vector table search:openblt_version */

#if EFI_USE_OPENBLT
#define getOpenBltVersion() (((uint32_t *)BLT_BIN_VERSION_ADDR)[0])
#endif
