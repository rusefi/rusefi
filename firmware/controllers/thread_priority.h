/**
 * @file        thread_priority.h
 * @brief       This file sets the thread priority for the threads running on rusEFI
 *
 * @date February 27, 2021
 * @author Matthew Kennedy, (c) 2021
 */

#pragma once

// ADC and ETB get highest priority - not much else actually runs the engine
#define PRIO_ADC (NORMALPRIO + 10)
#define PRIO_ETB (NORMALPRIO + 9)

// GPIO chips should be fast and go right back to sleep, plus can be timing sensitive
#define PRIO_GPIOCHIP (NORMALPRIO + 8)

// TX higher priority than RX because the ECU is generally the one transmitting the highest priority messages
#define PRIO_CAN_TX (NORMALPRIO + 7)
#define PRIO_CAN_RX (NORMALPRIO + 6)

// Less critical harware
#define PRIO_SERVO (NORMALPRIO + 5)
#define PRIO_STEPPER (NORMALPRIO + 5)
#define PRIO_CJ125 (NORMALPRIO + 5)

// Logging buffer flush is *slightly* above PRIO_CONSOLE so that we don't starve logging buffers during initialization and console commands
#define PRIO_TEXT_LOG (NORMALPRIO + 2)

// Console thread 
#define PRIO_CONSOLE (NORMALPRIO + 1)

// Less important things
#define PRIO_MMC (NORMALPRIO - 1)

// These can get starved without too much adverse effect
#define PRIO_AUX_SERIAL NORMALPRIO
#define PRIO_KNOCK_PROCESS (NORMALPRIO - 10)
#define PRIO_HIP9011 (NORMALPRIO - 10)

// These are intentionally low priority so they can't get in the way of anything else
#define PRIO_FLASH_WRITE LOWPRIO + 20

// USB mass storage
#define MSD_THD_PRIO LOWPRIO + 20

// Lua interpreter must be lowest priority, as the user's code may get stuck in an infinite loop
#define PRIO_LUA LOWPRIO + 10
