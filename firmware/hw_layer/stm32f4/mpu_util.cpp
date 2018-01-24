/**
 * @file	mpu_util.cpp
 *
 * @date Jul 27, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "mpu_util.h"
#include "error_handling.h"
#include "engine.h"
#include "pin_repository.h"
#include "stm32f4xx_hal_flash.h"
#include "rfiutil.h"

EXTERN_ENGINE;

extern "C" {
void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress);
}

extern uint32_t __main_stack_base__;

#define GET_CFSR() (*((volatile uint32_t *) (0xE000ED28)))

#if defined __GNUC__
// GCC version

typedef struct port_intctx intctx_t;

int getRemainingStack(thread_t *otp) {

#if CH_DBG_ENABLE_STACK_CHECK
	// this would dismiss coverity warning - see http://rusefi.com/forum/viewtopic.php?f=5&t=655
	// coverity[uninit_use]
	register intctx_t *r13 asm ("r13");
	otp->activeStack = r13;

	int remainingStack;
    if (ch.dbg.isr_cnt > 0) {
remainingStack = 9999;
		// ISR context
// todo		remainingStack = (int)(r13 - 1) - (int)&__main_stack_base__;
	} else {
remainingStack = 9999;
// todo		remainingStack = (int)(r13 - 1) - (int)otp->p_stklimit;
	}
	otp->remainingStack = remainingStack;
	return remainingStack;
#else
	return 99999;
#endif /* CH_DBG_ENABLE_STACK_CHECK */
}

#else /* __GNUC__ */

extern uint32_t CSTACK$$Base; /* symbol created by the IAR linker */
extern uint32_t IRQSTACK$$Base; /* symbol created by the IAR linker */

int getRemainingStack(thread_t *otp) {
#if CH_DBG_ENABLE_STACK_CHECK || defined(__DOXYGEN__)
	int remainingStack;
	if (ch.dbg.isr_cnt > 0) {
		remainingStack = (__get_SP() - sizeof(port_intctx)) - (int)&IRQSTACK$$Base;
	} else {
		remainingStack = (__get_SP() - sizeof(port_intctx)) - (int)otp->p_stklimit;
	}
	otp->remainingStack = remainingStack;
	return remainingStack;
#else
	return 999999;
#endif  
}

// IAR version

#endif /* GNU / IAR */

void baseHardwareInit(void) {
	// looks like this holds a random value on start? Let's set a nice clean zero
        DWT->CYCCNT = 0;

	BOR_Set(BOR_Level_1); // one step above default value
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

#if HAL_USE_SPI || defined(__DOXYGEN__)
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
		return boardConfiguration->spi1misoPin;
	case SPI_DEVICE_2:
		return boardConfiguration->spi2misoPin;
	case SPI_DEVICE_3:
		return boardConfiguration->spi3misoPin;
	default:
		break;
	}
	return GPIO_UNASSIGNED;
}

brain_pin_e getMosiPin(spi_device_e device) {
	switch(device) {
	case SPI_DEVICE_1:
		return boardConfiguration->spi1mosiPin;
	case SPI_DEVICE_2:
		return boardConfiguration->spi2mosiPin;
	case SPI_DEVICE_3:
		return boardConfiguration->spi3mosiPin;
	default:
		break;
	}
	return GPIO_UNASSIGNED;
}

brain_pin_e getSckPin(spi_device_e device) {
	switch(device) {
	case SPI_DEVICE_1:
		return boardConfiguration->spi1sckPin;
	case SPI_DEVICE_2:
		return boardConfiguration->spi2sckPin;
	case SPI_DEVICE_3:
		return boardConfiguration->spi3sckPin;
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
}

void initSpiModule(SPIDriver *driver, brain_pin_e sck, brain_pin_e miso,
		brain_pin_e mosi,
		int sckMode,
		int mosiMode,
		int misoMode) {

	efiSetPadMode("SPI clock", sck,	PAL_MODE_ALTERNATE(getSpiAf(driver)) + sckMode);

	efiSetPadMode("SPI master out", mosi, PAL_MODE_ALTERNATE(getSpiAf(driver)) + mosiMode);
	efiSetPadMode("SPI master in ", miso, PAL_MODE_ALTERNATE(getSpiAf(driver)) + misoMode);
}

void initSpiCs(SPIConfig *spiConfig, brain_pin_e csPin) {
	spiConfig->end_cb = NULL;
	ioportid_t port = getHwPort("spi", csPin);
	ioportmask_t pin = getHwPin("spi", csPin);
	spiConfig->ssport = port;
	spiConfig->sspad = pin;
	efiSetPadMode("chip select", csPin, PAL_STM32_MODE_OUTPUT);
}

#endif /* HAL_USE_SPI */

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

#if EFI_CAN_SUPPORT || defined(__DOXYGEN__)

static bool isValidCan1RxPin(brain_pin_e pin) {
	return pin == GPIOA_11 || pin == GPIOB_8 || pin == GPIOD_0;
}

static bool isValidCan1TxPin(brain_pin_e pin) {
	return pin == GPIOA_12 || pin == GPIOB_9 || GPIOD_1;
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
   if (isValidCan1RxPin(pinRx) && isValidCan1TxPin(pinTx))
      return &CAND1;
   if (isValidCan2RxPin(pinRx) && isValidCan2TxPin(pinTx))
      return &CAND2;
   return NULL;
}

#endif /* EFI_CAN_SUPPORT */

