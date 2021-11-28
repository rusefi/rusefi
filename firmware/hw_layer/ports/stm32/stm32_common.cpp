/**
 * @file	stm32_common.cpp
 * @brief	Low level common STM32 code
 *
 * @date Mar 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "expected.h"
#include "hardware.h"

#ifdef STM32F4XX
#include "stm32f4xx_hal_flash.h"
#elif defined(STM32F7XX)
#include "stm32f7xx_hal_flash.h"
#elif defined(STM32H7XX)
#include "stm32h7xx_hal_flash.h"
#endif

#define _2_MHZ 2'000'000

#if EFI_PROD_CODE
#include "mpu_util.h"
#include "backup_ram.h"
#endif /* EFI_PROD_CODE */

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
		return GPIOA_0;
	case EFI_ADC_1:
		return GPIOA_1;
	case EFI_ADC_2:
		return GPIOA_2;
	case EFI_ADC_3:
		return GPIOA_3;
	case EFI_ADC_4:
		return GPIOA_4;
	case EFI_ADC_5:
		return GPIOA_5;
	case EFI_ADC_6:
		return GPIOA_6;
	case EFI_ADC_7:
		return GPIOA_7;
	case EFI_ADC_8:
		return GPIOB_0;
	case EFI_ADC_9:
		return GPIOB_1;
	case EFI_ADC_10:
		return GPIOC_0;
	case EFI_ADC_11:
		return GPIOC_1;
	case EFI_ADC_12:
		return GPIOC_2;
	case EFI_ADC_13:
		return GPIOC_3;
	case EFI_ADC_14:
		return GPIOC_4;
	case EFI_ADC_15:
		return GPIOC_5;
	default:
		firmwareError(CUSTOM_ERR_ADC_UNKNOWN_CHANNEL, "Unknown hw channel %d [%s]", hwChannel, msg);
		return GPIO_INVALID;
	}
}

adc_channel_e getAdcChannel(brain_pin_e pin) {
	switch (pin) {
	case GPIOA_0:
		return EFI_ADC_0;
	case GPIOA_1:
		return EFI_ADC_1;
	case GPIOA_2:
		return EFI_ADC_2;
	case GPIOA_3:
		return EFI_ADC_3;
	case GPIOA_4:
		return EFI_ADC_4;
	case GPIOA_5:
		return EFI_ADC_5;
	case GPIOA_6:
		return EFI_ADC_6;
	case GPIOA_7:
		return EFI_ADC_7;
	case GPIOB_0:
		return EFI_ADC_8;
	case GPIOB_1:
		return EFI_ADC_9;
	case GPIOC_0:
		return EFI_ADC_10;
	case GPIOC_1:
		return EFI_ADC_11;
	case GPIOC_2:
		return EFI_ADC_12;
	case GPIOC_3:
		return EFI_ADC_13;
	case GPIOC_4:
		return EFI_ADC_14;
	case GPIOC_5:
		return EFI_ADC_15;
	case GPIO_UNASSIGNED:
		return EFI_ADC_NONE;
	default:
		firmwareError(OBD_PCM_Processor_Fault, "getAdcChannel %d", pin);
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

#if EFI_PROD_CODE

#if HAL_USE_PWM
namespace {
struct stm32_pwm_config {
	PWMDriver* const Driver;
	const uint8_t Channel;
	const uint8_t AlternateFunc;
};

class stm32_hardware_pwm : public hardware_pwm {
public:
	bool hasInit() const {
		return m_driver != nullptr;
	}

	// 2MHz, 16-bit timer gets us a usable frequency range of 31hz to 10khz
	static constexpr uint32_t c_timerFrequency = _2_MHZ;

	void start(const char* msg, const stm32_pwm_config& config, float frequency, float duty) {
		m_driver = config.Driver;
		m_channel = config.Channel;

		m_period = c_timerFrequency / frequency;

		// These timers are only 16 bit - don't risk overflow
		if (m_period > 0xFFF0) {
			firmwareError(CUSTOM_OBD_LOW_FREQUENCY, "PWM Frequency too low %f hz on pin \"%s\"", frequency, msg);
			return;
		}

		// If we have too few usable bits, we run out of resolution, so don't allow that either.
		// 200 counts = 0.5% resolution
		if (m_period < 200) {
			firmwareError(CUSTOM_OBD_HIGH_FREQUENCY, "PWM Frequency too high %d hz on pin \"%s\"", frequency, msg);
			return;
		}

		const PWMConfig pwmcfg = {
			c_timerFrequency,
			m_period,
			nullptr,
			{
				{PWM_OUTPUT_ACTIVE_HIGH, nullptr},
				{PWM_OUTPUT_ACTIVE_HIGH, nullptr},
				{PWM_OUTPUT_ACTIVE_HIGH, nullptr},
				{PWM_OUTPUT_ACTIVE_HIGH, nullptr}
			},
			0,
			0
		};

		// Start the timer running
		pwmStart(m_driver, &pwmcfg);

		// Set initial duty cycle
		setDuty(duty);
	}

	void setDuty(float duty) override {
		if (!m_driver) {
			firmwareError(OBD_PCM_Processor_Fault, "Attempted to set duty on null hard PWM device");
			return;
		}

		pwm_lld_enable_channel(m_driver, m_channel, getHighTime(duty));
	}

private:
	PWMDriver* m_driver = nullptr;
	uint8_t m_channel = 0;
	uint32_t m_period = 0;

	pwmcnt_t getHighTime(float duty) const {
		return m_period * duty;
	}
};
}

static expected<stm32_pwm_config> getConfigForPin(brain_pin_e pin) {
	switch (pin) {
#if STM32_PWM_USE_TIM1
	case GPIOA_8: return stm32_pwm_config{&PWMD1, 0, 1};
	case GPIOA_9: return stm32_pwm_config{&PWMD1, 1, 1};
	case GPIOA_10: return stm32_pwm_config{&PWMD1, 2, 1};
	case GPIOA_11: return stm32_pwm_config{&PWMD1, 3, 1};

	case GPIOE_9: return stm32_pwm_config{&PWMD1, 0, 1};
	case GPIOE_11: return stm32_pwm_config{&PWMD1, 1, 1};
	case GPIOE_13: return stm32_pwm_config{&PWMD1, 2, 1};
	case GPIOE_14: return stm32_pwm_config{&PWMD1, 3, 1};
#endif
#if STM32_PWM_USE_TIM2
	case GPIOA_15: return stm32_pwm_config{&PWMD2, 0, 1};
	case GPIOB_3: return stm32_pwm_config{&PWMD2, 1, 1};
	case GPIOB_10: return stm32_pwm_config{&PWMD2, 2, 1};
	case GPIOB_11: return stm32_pwm_config{&PWMD2, 3, 1};
#endif
#if STM32_PWM_USE_TIM3
	case GPIOB_4: return stm32_pwm_config{&PWMD3, 0, 2};
	case GPIOB_5: return stm32_pwm_config{&PWMD3, 1, 2};
#endif
#if STM32_PWM_USE_TIM4
	case GPIOB_6: return stm32_pwm_config{&PWMD4, 0, 2};
	case GPIOB_7: return stm32_pwm_config{&PWMD4, 1, 2};
	case GPIOB_8: return stm32_pwm_config{&PWMD4, 2, 2};
	case GPIOB_9: return stm32_pwm_config{&PWMD4, 3, 2};

	case GPIOD_12: return stm32_pwm_config{&PWMD4, 0, 2};
	case GPIOD_13: return stm32_pwm_config{&PWMD4, 1, 2};
	case GPIOD_14: return stm32_pwm_config{&PWMD4, 2, 2};
	case GPIOD_15: return stm32_pwm_config{&PWMD4, 3, 2};
#endif
#if STM32_PWM_USE_TIM5
	case GPIOA_0: return stm32_pwm_config{&PWMD5, 0, 2};
	case GPIOA_1: return stm32_pwm_config{&PWMD5, 1, 2};
	case GPIOA_2: return stm32_pwm_config{&PWMD5, 2, 2};
	case GPIOA_3: return stm32_pwm_config{&PWMD5, 3, 2};
#endif
#if STM32_PWM_USE_TIM8
	case GPIOC_6: return stm32_pwm_config{&PWMD8, 0, 3};
	case GPIOC_7: return stm32_pwm_config{&PWMD8, 1, 3};
	case GPIOC_8: return stm32_pwm_config{&PWMD8, 2, 3};
	case GPIOC_9: return stm32_pwm_config{&PWMD8, 3, 3};
#endif
	default: return unexpected;
	}
};

stm32_hardware_pwm pwms[5];

stm32_hardware_pwm* getNextPwmDevice() {
	for (size_t i = 0; i < efi::size(pwms); i++) {
		if (!pwms[i].hasInit()) {
			return &pwms[i];
		}
	}

	firmwareError(OBD_PCM_Processor_Fault, "Run out of hardware PWM devices!");
	return nullptr;
}

/*static*/ hardware_pwm* hardware_pwm::tryInitPin(const char* msg, brain_pin_e pin, float frequencyHz, float duty) {
	// Hardware PWM can't do very slow PWM - the timer counter is only 16 bits, so at 2MHz counting, that's a minimum of 31hz.
	if (frequencyHz < 50) {
		return nullptr;
	}

	auto cfg = getConfigForPin(pin);

	// This pin can't do hardware PWM
	if (!cfg) {
		return nullptr;
	}

	if (stm32_hardware_pwm* device = getNextPwmDevice()) {
		device->start(msg, cfg.Value, frequencyHz, duty);

		// Finally connect the timer to physical pin
		efiSetPadMode(msg, pin, PAL_MODE_ALTERNATE(cfg.Value.AlternateFunc));

		return device;
	}

	return nullptr;
}
#endif

void jump_to_bootloader() {
	#ifdef STM32H7XX
		// H7 needs a forcible reset of the USB peripheral(s) in order for the bootloader to work properly.
		// If you don't do this, the bootloader will execute, but USB doesn't work (nobody knows why)
		// See https://community.st.com/s/question/0D53W00000vQEWsSAO/stm32h743-dfu-entry-doesnt-work-unless-boot0-held-high-at-poweron
		RCC->AHB1ENR &= ~(RCC_AHB1ENR_USB1OTGHSEN | RCC_AHB1ENR_USB2OTGFSEN);
	#endif

	// leave DFU breadcrumb which assembly startup code would check, see [rusefi][DFU] section in assembly code
	*((unsigned long *)0x2001FFF0) = 0xDEADBEEF; // End of RAM
	// and now reboot
	NVIC_SystemReset();
}
#endif /* EFI_PROD_CODE */

#if EFI_AUX_SERIAL

static bool isValidUART6TxPin(brain_pin_e pin) {
	return pin == GPIOC_6 || pin == GPIOG_14;
}

static bool isValidUART6RxPin(brain_pin_e pin) {
	return pin == GPIOC_7 || pin == GPIOG_9;
}

bool isValidSerialTxPin(brain_pin_e pin) {
   return isValidUART6TxPin(pin);
}

bool isValidSerialRxPin(brain_pin_e pin) {
   return isValidUART6RxPin(pin);
}

#endif /*EFI_AUX_SERIAL*/

#if EFI_PROD_CODE

BOR_Level_t BOR_Get(void) {
	FLASH_OBProgramInitTypeDef FLASH_Handle;

	/* Read option bytes */
	HAL_FLASHEx_OBGetConfig(&FLASH_Handle);

	/* Return BOR value */
	return (BOR_Level_t) FLASH_Handle.BORLevel;
}

BOR_Result_t BOR_Set(BOR_Level_t BORValue) {
	if (BOR_Get() == BORValue) {
		return BOR_Result_Ok;
	}


	FLASH_OBProgramInitTypeDef FLASH_Handle;

	FLASH_Handle.BORLevel = (uint32_t)BORValue;
	FLASH_Handle.OptionType = OPTIONBYTE_BOR;

	HAL_FLASH_OB_Unlock();

	HAL_FLASHEx_OBProgram(&FLASH_Handle);

	HAL_StatusTypeDef status = HAL_FLASH_OB_Launch();

	HAL_FLASH_OB_Lock();

	if (status != HAL_OK) {
		return BOR_Result_Error;
	}

	return BOR_Result_Ok;
}

void baseMCUInit(void) {
	// looks like this holds a random value on start? Let's set a nice clean zero
	DWT->CYCCNT = 0;

	BOR_Set(BOR_Level_1); // one step above default value
}


extern "C" {
void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress);
}

extern uint32_t __main_stack_base__;

#define GET_CFSR() (*((volatile uint32_t *) (0xE000ED28)))

typedef struct port_intctx intctx_t;

EXTERNC int getRemainingStack(thread_t *otp) {

#if CH_DBG_ENABLE_STACK_CHECK
	// this would dismiss coverity warning - see http://rusefi.com/forum/viewtopic.php?f=5&t=655
	// coverity[uninit_use]
	register intctx_t *r13 asm ("r13");
	otp->activeStack = r13;

	int remainingStack;
    if (ch.dbg.isr_cnt > 0) {
		// ISR context
		remainingStack = (int)(r13 - 1) - (int)&__main_stack_base__;
	} else {
		remainingStack = (int)(r13 - 1) - (int)otp->wabase;
	}
	otp->remainingStack = remainingStack;
	return remainingStack;
#else
	UNUSED(otp);
	return 99999;
#endif /* CH_DBG_ENABLE_STACK_CHECK */
}

void _unhandled_exception(void) {
/*lint -restore*/

  chDbgPanic3("_unhandled_exception", __FILE__, __LINE__);
  while (true) {
  }
}

void DebugMonitorVector(void) {
	chDbgPanic3("DebugMonitorVector", __FILE__, __LINE__);
	while (TRUE)
		;
}

void UsageFaultVector(void) {
	chDbgPanic3("UsageFaultVector", __FILE__, __LINE__);
	while (TRUE)
		;
}

void BusFaultVector(void) {
	chDbgPanic3("BusFaultVector", __FILE__, __LINE__);
	while (TRUE) {
	}
}

/**
 + * @brief   Register values for postmortem debugging.
 + */
volatile uint32_t postmortem_r0;
volatile uint32_t postmortem_r1;
volatile uint32_t postmortem_r2;
volatile uint32_t postmortem_r3;
volatile uint32_t postmortem_r12;
volatile uint32_t postmortem_lr; /* Link register. */
volatile uint32_t postmortem_pc; /* Program counter. */
volatile uint32_t postmortem_psr;/* Program status register. */
volatile uint32_t postmortem_CFSR;
volatile uint32_t postmortem_HFSR;
volatile uint32_t postmortem_DFSR;
volatile uint32_t postmortem_AFSR;
volatile uint32_t postmortem_BFAR;
volatile uint32_t postmortem_MMAR;
volatile uint32_t postmortem_SCB_SHCSR;

/**
 * @brief   Evaluates to TRUE if system runs under debugger control.
 * @note    This bit resets only by power reset.
 */
#define is_under_debugger() (((CoreDebug)->DHCSR) & \
                            CoreDebug_DHCSR_C_DEBUGEN_Msk)

/**
 *
 */
void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress) {

	postmortem_r0 = pulFaultStackAddress[0];
	postmortem_r1 = pulFaultStackAddress[1];
	postmortem_r2 = pulFaultStackAddress[2];
	postmortem_r3 = pulFaultStackAddress[3];
	postmortem_r12 = pulFaultStackAddress[4];
	postmortem_lr = pulFaultStackAddress[5];
	postmortem_pc = pulFaultStackAddress[6];
	postmortem_psr = pulFaultStackAddress[7];

	/* Configurable Fault Status Register. Consists of MMSR, BFSR and UFSR */
	postmortem_CFSR = GET_CFSR();

	/* Hard Fault Status Register */
	postmortem_HFSR = (*((volatile uint32_t *) (0xE000ED2C)));

	/* Debug Fault Status Register */
	postmortem_DFSR = (*((volatile uint32_t *) (0xE000ED30)));

	/* Auxiliary Fault Status Register */
	postmortem_AFSR = (*((volatile uint32_t *) (0xE000ED3C)));

	/* Read the Fault Address Registers. These may not contain valid values.
	 Check BFARVALID/MMARVALID to see if they are valid values
	 MemManage Fault Address Register */
	postmortem_MMAR = (*((volatile uint32_t *) (0xE000ED34)));
	/* Bus Fault Address Register */
	postmortem_BFAR = (*((volatile uint32_t *) (0xE000ED38)));

	postmortem_SCB_SHCSR = SCB->SHCSR;

	if (is_under_debugger()) {
		__asm("BKPT #0\n");
		// Break into the debugger
	}

	/* harmless infinite loop */
	while (1) {
		;
	}
}

void HardFaultVector(void) {
#if 0 && defined __GNUC__
	__asm volatile (
			" tst lr, #4                                                \n"
			" ite eq                                                    \n"
			" mrseq r0, msp                                             \n"
			" mrsne r0, psp                                             \n"
			" ldr r1, [r0, #24]                                         \n"
			" ldr r2, handler2_address_const                            \n"
			" bx r2                                                     \n"
			" handler2_address_const: .word prvGetRegistersFromStack    \n"
	);

#else
#endif /* 0 && defined __GNUC__ */

	int cfsr = GET_CFSR();
	if (cfsr & 0x1) {
		chDbgPanic3("H IACCVIOL", __FILE__, __LINE__);
	} else if (cfsr & 0x100) {
		chDbgPanic3("H IBUSERR", __FILE__, __LINE__);
	} else if (cfsr & 0x20000) {
		chDbgPanic3("H INVSTATE", __FILE__, __LINE__);
	} else {
		chDbgPanic3("HardFaultVector", __FILE__, __LINE__);
	}

	while (TRUE) {
	}
}

#if HAL_USE_SPI
bool isSpiInitialized[5] = { false, false, false, false, false };

static int getSpiAf(SPIDriver *driver) {
#if STM32_SPI_USE_SPI1
	if (driver == &SPID1) {
		return EFI_SPI1_AF;
	}
#endif
#if STM32_SPI_USE_SPI2
	if (driver == &SPID2) {
		return EFI_SPI2_AF;
	}
#endif
#if STM32_SPI_USE_SPI3
	if (driver == &SPID3) {
		return EFI_SPI3_AF;
	}
#endif
	return -1;
}

brain_pin_e getMisoPin(spi_device_e device) {
	switch(device) {
	case SPI_DEVICE_1:
		return engineConfiguration->spi1misoPin;
	case SPI_DEVICE_2:
		return engineConfiguration->spi2misoPin;
	case SPI_DEVICE_3:
		return engineConfiguration->spi3misoPin;
	default:
		break;
	}
	return GPIO_UNASSIGNED;
}

brain_pin_e getMosiPin(spi_device_e device) {
	switch(device) {
	case SPI_DEVICE_1:
		return engineConfiguration->spi1mosiPin;
	case SPI_DEVICE_2:
		return engineConfiguration->spi2mosiPin;
	case SPI_DEVICE_3:
		return engineConfiguration->spi3mosiPin;
	default:
		break;
	}
	return GPIO_UNASSIGNED;
}

brain_pin_e getSckPin(spi_device_e device) {
	switch(device) {
	case SPI_DEVICE_1:
		return engineConfiguration->spi1sckPin;
	case SPI_DEVICE_2:
		return engineConfiguration->spi2sckPin;
	case SPI_DEVICE_3:
		return engineConfiguration->spi3sckPin;
	default:
		break;
	}
	return GPIO_UNASSIGNED;
}

void turnOnSpi(spi_device_e device) {
	if (isSpiInitialized[device])
		return; // already initialized
	isSpiInitialized[device] = true;
	if (device == SPI_DEVICE_1) {
// todo: introduce a nice structure with all fields for same SPI
#if STM32_SPI_USE_SPI1
//	scheduleMsg(&logging, "Turning on SPI1 pins");
		initSpiModule(&SPID1, getSckPin(device),
				getMisoPin(device),
				getMosiPin(device),
				engineConfiguration->spi1SckMode,
				engineConfiguration->spi1MosiMode,
				engineConfiguration->spi1MisoMode);
#endif /* STM32_SPI_USE_SPI1 */
	}
	if (device == SPI_DEVICE_2) {
#if STM32_SPI_USE_SPI2
//	scheduleMsg(&logging, "Turning on SPI2 pins");
		initSpiModule(&SPID2, getSckPin(device),
				getMisoPin(device),
				getMosiPin(device),
				engineConfiguration->spi2SckMode,
				engineConfiguration->spi2MosiMode,
				engineConfiguration->spi2MisoMode);
#endif /* STM32_SPI_USE_SPI2 */
	}
	if (device == SPI_DEVICE_3) {
#if STM32_SPI_USE_SPI3
//	scheduleMsg(&logging, "Turning on SPI3 pins");
		initSpiModule(&SPID3, getSckPin(device),
				getMisoPin(device),
				getMosiPin(device),
				engineConfiguration->spi3SckMode,
				engineConfiguration->spi3MosiMode,
				engineConfiguration->spi3MisoMode);
#endif /* STM32_SPI_USE_SPI3 */
	}
	if (device == SPI_DEVICE_4) {
#if STM32_SPI_USE_SPI4
//		scheduleMsg(&logging, "Turning on SPI4 pins");
		/* there is no cofiguration fields for SPI4 in engineConfiguration, rely on board init code
		 * it should set proper functions for SPI4 pins */
#endif /* STM32_SPI_USE_SPI4 */
	}
}

void initSpiModule(SPIDriver *driver, brain_pin_e sck, brain_pin_e miso,
		brain_pin_e mosi,
		int sckMode,
		int mosiMode,
		int misoMode) {

	/**
	 * See https://github.com/rusefi/rusefi/pull/664/
	 *
	 * Info on the silicon defect can be found in this document, section 2.5.2:
	 * https://www.st.com/content/ccc/resource/technical/document/errata_sheet/0a/98/58/84/86/b6/47/a2/DM00037591.pdf/files/DM00037591.pdf/jcr:content/translations/en.DM00037591.pdf
	 */
	efiSetPadMode("SPI clock", sck,	PAL_MODE_ALTERNATE(getSpiAf(driver)) | sckMode | PAL_STM32_OSPEED_HIGHEST);

	efiSetPadMode("SPI master out", mosi, PAL_MODE_ALTERNATE(getSpiAf(driver)) | mosiMode | PAL_STM32_OSPEED_HIGHEST);

	// Activate the internal pullup on MISO: SD cards indicate "busy" by holding MOSI low,
	// so in case there is no SD card installed, the line could float low and indicate that
	// the (non existent) card is busy.  We pull the line high to indicate "not busy" in case
	// of a missing card.
	efiSetPadMode("SPI master in ", miso, PAL_MODE_ALTERNATE(getSpiAf(driver)) | misoMode | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_PULLUP);
}

void initSpiCs(SPIConfig *spiConfig, brain_pin_e csPin) {
	spiConfig->end_cb = nullptr;
	ioportid_t port = getHwPort("spi", csPin);
	ioportmask_t pin = getHwPin("spi", csPin);
	spiConfig->ssport = port;
	spiConfig->sspad = pin;
	efiSetPadMode("chip select", csPin, PAL_STM32_MODE_OUTPUT);
}

#ifdef STM32H7XX
// H7 SPI clock is set to 80MHz
// fast mode is 80mhz/2 = 40MHz
SPIConfig mmc_hs_spicfg = {
		.circular = false,
		.end_cb = NULL,
		.ssport = NULL,
		.sspad = 0,
		.cfg1 = 7 // 8 bits per byte
			| 0 /* MBR = 0, divider = 2 */,
		.cfg2 = 0
};

// Slow mode is 80mhz/4 = 20MHz
SPIConfig mmc_ls_spicfg = {
		.circular = false,
		.end_cb = NULL,
		.ssport = NULL,
		.sspad = 0,
		.cfg1 = 7 // 8 bits per byte
			| SPI_CFG1_MBR_0 /* MBR = 001, divider = 4 */,
		.cfg2 = 0
};

#else /* not STM32H7XX */

// SD cards are good up to 25MHz in "slow" mode, and 50MHz in "fast" mode
// 168mhz F4:
// Slow mode is 10.5 or 5.25 MHz, depending on which SPI device
// Fast mode is 42 or 21 MHz
// 216mhz F7:
// Slow mode is 13.5 or 6.75 MHz
// Fast mode is 54 or 27 MHz (technically out of spec, needs testing!)
SPIConfig mmc_hs_spicfg = {
		.circular = false,
		.end_cb = NULL,
		.ssport = NULL,
		.sspad = 0,
		.cr1 = SPI_BaudRatePrescaler_2,
		.cr2 = 0
};

SPIConfig mmc_ls_spicfg = {
		.circular = false,
		.end_cb = NULL,
		.ssport = NULL,
		.sspad = 0,
		.cr1 = SPI_BaudRatePrescaler_8,
		.cr2 = 0
};
#endif

#endif /* HAL_USE_SPI */

#if EFI_CAN_SUPPORT

static bool isValidCan1RxPin(brain_pin_e pin) {
	return pin == GPIOA_11 || pin == GPIOB_8 || pin == GPIOD_0;
}

static bool isValidCan1TxPin(brain_pin_e pin) {
	return pin == GPIOA_12 || pin == GPIOB_9 || pin == GPIOD_1;
}

static bool isValidCan2RxPin(brain_pin_e pin) {
	return pin == GPIOB_5 || pin == GPIOB_12;
}

static bool isValidCan2TxPin(brain_pin_e pin) {
	return pin == GPIOB_6 || pin == GPIOB_13;
}

bool isValidCanTxPin(brain_pin_e pin) {
   return isValidCan1TxPin(pin) || isValidCan2TxPin(pin);
}

bool isValidCanRxPin(brain_pin_e pin) {
   return isValidCan1RxPin(pin) || isValidCan2RxPin(pin);
}

CANDriver * detectCanDevice(brain_pin_e pinRx, brain_pin_e pinTx) {
#if STM32_CAN_USE_CAN1 || STM32_CAN_USE_FDCAN1
   if (isValidCan1RxPin(pinRx) && isValidCan1TxPin(pinTx))
      return &CAND1;
#endif
#if STM32_CAN_USE_CAN2 || STM32_CAN_USE_FDCAN2
   if (isValidCan2RxPin(pinRx) && isValidCan2TxPin(pinTx))
      return &CAND2;
#endif
   return NULL;
}

#endif /* EFI_CAN_SUPPORT */

#endif // EFI_PROD_CODE
