/**
 * @file        stm32_icu.cpp
 * @brief       Port implementation for the STM32 timer units in ICU mode found on the STM32F4 and STM32F7
 *
 * @date October 20, 2022
 * @author Andrey Gusakov, (c) 2022
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if HAL_USE_ICU

uint32_t getAlternateFunctions(ICUDriver *driver) {
#if STM32_ICU_USE_TIM1
	if (driver == &ICUD1) {
		return GPIO_AF_TIM1;
	}
#endif
#if STM32_ICU_USE_TIM2
	if (driver == &ICUD2) {
		return GPIO_AF_TIM2;
	}
#endif
#if STM32_ICU_USE_TIM3
	if (driver == &ICUD3) {
		return GPIO_AF_TIM3;
	}
#endif
#if STM32_ICU_USE_TIM4
	if (driver == &ICUD4) {
		return GPIO_AF_TIM4;
	}
#endif
#if STM32_ICU_USE_TIM8
	if (driver == &ICUD8) {
		return GPIO_AF_TIM8;
	}
#endif
#if STM32_ICU_USE_TIM9
	if (driver == &ICUD9) {
		return GPIO_AF_TIM9;
	}
#endif
	if (driver == NULL) {
		//firmwareError(CUSTOM_ERR_ICU_AF, "getAlternateFunctions(NULL)");
	} else {
		//firmwareError(CUSTOM_ERR_ICU_DRIVER, "No such driver");
	}

	return 0xffffffff;
}

#define	RETURN_ICU_TRUE(icu, channel, af)				\
	do {												\
		if (icu_ptr) {									\
			if (*icu_ptr != (icu)) {					\
				*icu_ptr = (icu);						\
				if (channel_ptr)						\
					*channel_ptr = (channel);			\
				if (af_ptr)								\
					*af_ptr = (af);						\
				return true;							\
			} else {									\
				/* if current icu is allready in *icu_ptr, continue and return another icu available on this pin, if any */ \
			}											\
		} else {										\
			/* called with null icu_ptr, just to know if icu is available on given pin */ \
			return true;								\
		}												\
	} while(0)

#if (STM32_ICU_USE_TIM1 == TRUE)
	#define RETURN_ICU1(channel)	RETURN_ICU_TRUE(&ICUD1, channel, GPIO_AF_TIM1)
#else
	#define RETURN_ICU1(channel)
#endif

#if (STM32_ICU_USE_TIM2 == TRUE)
	#define RETURN_ICU2(channel)	RETURN_ICU_TRUE(&ICUD2, channel, GPIO_AF_TIM2)
#else
	#define RETURN_ICU2(channel)
#endif

#if (STM32_ICU_USE_TIM3 == TRUE)
	#define RETURN_ICU3(channel)	RETURN_ICU_TRUE(&ICUD3, channel, GPIO_AF_TIM3)
#else
	#define RETURN_ICU3(channel)
#endif

#if (STM32_ICU_USE_TIM4 == TRUE)
	#define RETURN_ICU4(channel)	RETURN_ICU_TRUE(&ICUD4, channel, GPIO_AF_TIM4)
#else
	#define RETURN_ICU4(channel)
#endif

#if (STM32_ICU_USE_TIM5 == TRUE)
	#define RETURN_ICU5(channel)	RETURN_ICU_TRUE(&ICUD5, channel, GPIO_AF_TIM5)
#else
	#define RETURN_ICU5(channel)
#endif

/* TIM6 is internal only */

/* TIM7 is internal only */

#if (STM32_ICU_USE_TIM8 == TRUE)
	#define RETURN_ICU8(channel)	RETURN_ICU_TRUE(&ICUD8, channel, GPIO_AF_TIM8)
#else
	#define RETURN_ICU8(channel)
#endif

#if (STM32_ICU_USE_TIM9 == TRUE)
	#define RETURN_ICU9(channel)	RETURN_ICU_TRUE(&ICUD9, channel, GPIO_AF_TIM9)
#else
	#define RETURN_ICU9(channel)
#endif

#if (STM32_ICU_USE_TIM10 == TRUE)
	#define RETURN_ICU10(channel)	RETURN_ICU_TRUE(&ICUD10, channel, GPIO_AF_TIM10)
#else
	#define RETURN_ICU10(channel)
#endif

#if (STM32_ICU_USE_TIM11 == TRUE)
	#define RETURN_ICU11(channel)	RETURN_ICU_TRUE(&ICUD11, channel, GPIO_AF_TIM11)
#else
	#define RETURN_ICU11(channel)
#endif

#if (STM32_ICU_USE_TIM12 == TRUE)
	#define RETURN_ICU12(channel)	RETURN_ICU_TRUE(&ICUD12, channel, GPIO_AF_TIM12)
#else
	#define RETURN_ICU12(channel)
#endif

#if (STM32_ICU_USE_TIM13 == TRUE)
	#define RETURN_ICU13(channel)	RETURN_ICU_TRUE(&ICUD13, channel, GPIO_AF_TIM13)
#else
	#define RETURN_ICU13(channel)
#endif

#if (STM32_ICU_USE_TIM14 == TRUE)
	#define RETURN_ICU14(channel)	RETURN_ICU_TRUE(&ICUD14, channel, GPIO_AF_TIM14)
#else
	#define RETURN_ICU14(channel)
#endif

/**
 * ChibiOS limitation is that only channels #1 and #2 could be used for input capture
 */
bool getIcuParams(brain_pin_e hwPin, ICUDriver ** icu_ptr, icuchannel_t *channel_ptr, iomode_t *af_ptr)
{
	switch (hwPin) {
		case Gpio::A0:
			RETURN_ICU5(ICU_CHANNEL_1);
			return false;
		case Gpio::A1:
			RETURN_ICU2(ICU_CHANNEL_2);
			RETURN_ICU5(ICU_CHANNEL_2);
			return false;
		case Gpio::A2:
			RETURN_ICU9(ICU_CHANNEL_1);
			return false;
		case Gpio::A3:
			RETURN_ICU9(ICU_CHANNEL_2);
			return false;
		case Gpio::A5:
			RETURN_ICU2(ICU_CHANNEL_1);
			return false;
		case Gpio::A6:
			RETURN_ICU3(ICU_CHANNEL_1);
			RETURN_ICU13(ICU_CHANNEL_1);
			return false;
		case Gpio::A7:
			RETURN_ICU3(ICU_CHANNEL_2);
			RETURN_ICU14(ICU_CHANNEL_1);
			return false;
		case Gpio::A8:
			RETURN_ICU1(ICU_CHANNEL_1);
			return false;
		case Gpio::A9:
			RETURN_ICU1(ICU_CHANNEL_2);
			return false;
		case Gpio::A15:
			RETURN_ICU2(ICU_CHANNEL_1);
			return false;
		case Gpio::B3:
			RETURN_ICU2(ICU_CHANNEL_2);
			return false;
		case Gpio::B4:
			RETURN_ICU3(ICU_CHANNEL_1);
			return false;
		case Gpio::B5:
			RETURN_ICU3(ICU_CHANNEL_2);
			return false;
		case Gpio::B6:
			RETURN_ICU4(ICU_CHANNEL_1);
			return false;
		case Gpio::B7:
			RETURN_ICU4(ICU_CHANNEL_2);
			return false;
		case Gpio::B8:
			RETURN_ICU10(ICU_CHANNEL_1);
			return false;
		case Gpio::B9:
			RETURN_ICU11(ICU_CHANNEL_1);
			return false;
		case Gpio::B14:
			RETURN_ICU12(ICU_CHANNEL_1);
			return false;
		case Gpio::B15:
			RETURN_ICU12(ICU_CHANNEL_2);
			return false;
		case Gpio::C6:
			RETURN_ICU3(ICU_CHANNEL_1);
			RETURN_ICU8(ICU_CHANNEL_1);
			return false;
		case Gpio::C7:
			RETURN_ICU3(ICU_CHANNEL_2);
			RETURN_ICU8(ICU_CHANNEL_2);
			return false;
		case Gpio::D12:
			RETURN_ICU4(ICU_CHANNEL_1);
			return false;
		case Gpio::D13:
			RETURN_ICU4(ICU_CHANNEL_2);
			return false;
		case Gpio::E5:
			RETURN_ICU9(ICU_CHANNEL_1);
			return false;
		case Gpio::E6:
			RETURN_ICU9(ICU_CHANNEL_2);
			return false;
		case Gpio::E9:
			RETURN_ICU1(ICU_CHANNEL_1);
			return false;
		case Gpio::E11:
			RETURN_ICU1(ICU_CHANNEL_2);
			return false;
		case Gpio::F6:
			RETURN_ICU10(ICU_CHANNEL_1);
			return false;
		case Gpio::F7:
			RETURN_ICU11(ICU_CHANNEL_1);
			return false;
		case Gpio::F8:
			RETURN_ICU13(ICU_CHANNEL_1);
			return false;
		case Gpio::F9:
			RETURN_ICU14(ICU_CHANNEL_1);
			return false;
		case Gpio::H6:
			RETURN_ICU12(ICU_CHANNEL_1);
			return false;
		case Gpio::H9:
			RETURN_ICU12(ICU_CHANNEL_2);
			return false;
		case Gpio::H10:
			RETURN_ICU5(ICU_CHANNEL_1);
			return false;
		case Gpio::H11:
			RETURN_ICU5(ICU_CHANNEL_2);
			return false;
		case Gpio::I5:
			RETURN_ICU8(ICU_CHANNEL_1);
			return false;
		case Gpio::I6:
			RETURN_ICU8(ICU_CHANNEL_2);
			return false;

		default:
			return false;
	}

	return false;
}

#endif //HAL_USE_ICU
