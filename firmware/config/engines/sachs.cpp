/**
 * @file	sachs.cpp
 *
 * set_engine_type 29
 * http://rusefi.com/forum/viewtopic.php?f=3&t=396
 *
 * @date Jan 26, 2015
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "sachs.h"

void setSachs(engine_configuration_s *engineConfiguration) {
	engineConfiguration->displacement = 0.1; // 100cc

	// Frankenstein analog input #1: PA1 adc1
	// Frankenstein analog input #2: PA3 adc3
	// Frankenstein analog input #3: PC3 adc13
	// Frankenstein analog input #4: PC1 adc11
	// Frankenstein analog input #5: PA0 adc0
	// Frankenstein analog input #6: PC2 adc12
	// Frankenstein analog input #7: PA4 adc4
	// Frankenstein analog input #8: PA2 adc2
	// Frankenstein analog input #9: PA6 adc6
	// Frankenstein analog input #10: PA7 adc7
	// Frankenstein analog input #11: PC4 adc14
	// Frankenstein analog input #12: PC5 adc15



	// Frankenstein: low side - out #1: PC14
	// Frankenstein: low side - out #2: PC15
	// Frankenstein: low side - out #3: PE6
	// Frankenstein: low side - out #4: PC13
	// Frankenstein: low side - out #5: PE4
	// Frankenstein: low side - out #6: PE5
	// Frankenstein: low side - out #7: PE2
	// Frankenstein: low side - out #8: PE3
	// Frankenstein: low side - out #9: PE0
	// Frankenstein: low side - out #10: PE1
	// Frankenstein: low side - out #11: PB8
	// Frankenstein: low side - out #12: PB9


}

