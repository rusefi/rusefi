/**
 * @file	stm32_i2c.cpp
 * @brief	Low level common STM32 I2C code
 *
 * @date Jul 10, 2026
 * @author Andrey Gusakov
 */

#include "pch.h"
#include "stm32_i2c.h"

#if HAL_USE_I2C

#ifndef EFI_I2C1_AF
#define EFI_I2C1_AF 4
#endif
#ifndef EFI_I2C2_AF
#define EFI_I2C2_AF 4
#endif
#ifndef EFI_I2C3_AF
#define EFI_I2C3_AF 4
#endif
#ifndef EFI_I2C4_AF
#define EFI_I2C4_AF 4
#endif

static int getI2cAf(I2CDriver *driver) {
#if STM32_I2C_USE_I2C1
	if (driver == &I2CD1) {
		return EFI_I2C1_AF;
	}
#endif
#if STM32_I2C_USE_I2C2
	if (driver == &I2CD2) {
		return EFI_I2C2_AF;
	}
#endif
#if STM32_I2C_USE_I2C3
	if (driver == &I2CD3) {
		return EFI_I2C3_AF;
	}
#endif
#if STM32_I2C_USE_I2C4
	if (driver == &I2CD4) {
		return EFI_I2C4_AF;
	}
#endif
	criticalError("I2C AF not available");
	return -1;
}

// Defaults
#if defined(STM32F7)
/* TODO: calculate real values! */
static const I2CConfig i2cfgs[] =
{
	[i2c_speed_e::I2C_SPEED_100K] = {
		.timingr	 = STM32_TIMINGR_PRESC(15U) | STM32_TIMINGR_SCLDEL(4U) |
					   STM32_TIMINGR_SDADEL(2U) | STM32_TIMINGR_SCLH(15U) |
					   STM32_TIMINGR_SCLL(21U),
		.cr1		 = 0,
		.cr2		 = 0
	},
	[i2c_speed_e::I2C_SPEED_400K] = {
		.timingr	 = STM32_TIMINGR_PRESC(15U) | STM32_TIMINGR_SCLDEL(4U) |
					   STM32_TIMINGR_SDADEL(2U) | STM32_TIMINGR_SCLH(15U) |
					   STM32_TIMINGR_SCLL(21U),
		.cr1		 = 0,
		.cr2		 = 0
	},
	[i2c_speed_e::I2C_SPEED_1M] = {
		.timingr	 = STM32_TIMINGR_PRESC(15U) | STM32_TIMINGR_SCLDEL(4U) |
					   STM32_TIMINGR_SDADEL(2U) | STM32_TIMINGR_SCLH(15U) |
					   STM32_TIMINGR_SCLL(21U),
		.cr1		 = 0,
		.cr2		 = 0
	},
	[i2c_speed_e::I2C_SPEED_3p4M] = {
		.timingr	 = STM32_TIMINGR_PRESC(15U) | STM32_TIMINGR_SCLDEL(4U) |
					   STM32_TIMINGR_SDADEL(2U) | STM32_TIMINGR_SCLH(15U) |
					   STM32_TIMINGR_SCLL(21U),
		.cr1		 = 0,
		.cr2		 = 0
	},
	[i2c_speed_e::I2C_SPEED_5M] = {
		.timingr	 = STM32_TIMINGR_PRESC(15U) | STM32_TIMINGR_SCLDEL(4U) |
					   STM32_TIMINGR_SDADEL(2U) | STM32_TIMINGR_SCLH(15U) |
					   STM32_TIMINGR_SCLL(21U),
		.cr1		 = 0,
		.cr2		 = 0
	}
};
#else
static const I2CConfig i2cfgs[] =
{
	[i2c_speed_e::I2C_SPEED_100K] = {
		.op_mode     = OPMODE_I2C,
		.clock_speed = 100000,
		.duty_cycle  = STD_DUTY_CYCLE,
	},
	[i2c_speed_e::I2C_SPEED_400K] = {
		.op_mode     = OPMODE_I2C,
		.clock_speed = 400000,
		.duty_cycle  = FAST_DUTY_CYCLE_2,
	},
	[i2c_speed_e::I2C_SPEED_1M] = {
		.op_mode     = OPMODE_I2C,
		.clock_speed = 1000000,
		.duty_cycle  = FAST_DUTY_CYCLE_2,
	},
	[i2c_speed_e::I2C_SPEED_3p4M] = {
		.op_mode     = OPMODE_I2C,
		.clock_speed = 3400000,
		.duty_cycle  = FAST_DUTY_CYCLE_2,
	},
	[i2c_speed_e::I2C_SPEED_5M] = {
		.op_mode     = OPMODE_I2C,
		.clock_speed = 5000000,
		.duty_cycle  = FAST_DUTY_CYCLE_2,
	}
};
#endif

bool initI2cModule(I2CDriver *driver, brain_pin_e scl, brain_pin_e sda, i2c_speed_e speed) {
	if (!isBrainPinValid(scl) || !isBrainPinValid(sda)) {
		criticalError("I2C pins not valid");
		return false;
	}

	int af = getI2cAf(driver);

	efiSetPadMode("I2C SCL", scl, PAL_MODE_ALTERNATE(af) | PAL_STM32_OTYPE_OPENDRAIN);
	efiSetPadMode("I2C SDA", sda, PAL_MODE_ALTERNATE(af) | PAL_STM32_OTYPE_OPENDRAIN);

	return (i2cStart(driver, &i2cfgs[speed]) == HAL_RET_SUCCESS);
}

void deinitI2cModule(I2CDriver *driver) {
	i2cStop(driver);
}

#endif /* HAL_USE_I2C */
