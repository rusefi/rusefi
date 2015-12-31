/**
 * @file    snow_blower.h
 * @brief	Default configuration of a single-cylinder engine

 * @date Sep 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#ifndef SNOW_BLOWER_H_
#define SNOW_BLOWER_H_

#if EFI_ENGINE_SNOW_BLOWER

#define NUMBER_OF_CYLINDERS 1

#define EFI_ENGINE_ID "Snow Blower"

#define RPM_MULT (1)

#define getCoolantTemperature() 0
#define getIntakeAirTemperature() 0

#define getMaf() 0
#define getTPS() 0
#define getAfr() 0

#endif /* EFI_ENGINE_SNOW_BLOWER */

#endif /* SNOW_BLOWER_H_ */
