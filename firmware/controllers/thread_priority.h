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

// Console thread 
#define PRIO_CONSOLE (NORMALPRIO + 1)

// Less important things
#define PRIO_MMC (NORMALPRIO - 1)

// These can get starved without too much adverse effect
#define PRIO_AUX_SERIAL NORMALPRIO
#define PRIO_KNOCK_PROCESS (NORMALPRIO - 10)
#define PRIO_HIP9011 (NORMALPRIO - 10)
