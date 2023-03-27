/*
 * @file mercedes.cpp
 *
 * @date: Sep 17, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"
#if HW_HELLEN
#include "hellen_meta.h"
#endif // HW_HELLEN

static void common() {

}

void setHellenMercedes128_4_cyl() {
    common();
#if HW_HELLEN
	engineConfiguration->injectionPins[0] = H144_LS_7;
	engineConfiguration->injectionPins[1] = H176_LS_6;
	engineConfiguration->injectionPins[2] = H176_LS_8;
	engineConfiguration->injectionPins[3] = H176_LS_5;
	engineConfiguration->injectionPins[4] = Gpio::Unassigned;
	engineConfiguration->injectionPins[5] = Gpio::Unassigned;
	engineConfiguration->injectionPins[6] = Gpio::Unassigned;
	engineConfiguration->injectionPins[7] = Gpio::Unassigned;
#endif // HW_HELLEN
}

// is this M104 or M112 or both?
void setHellenMercedes128_6_cyl() {
    common();
	engineConfiguration->cylindersCount = 6;
	// 1-4-2-5-3-6 M104
	engineConfiguration->firingOrder = FO_1_4_3_6_2_5; // M112
}

// M113
void setHellenMercedes128_8_cyl() {
    common();
	engineConfiguration->cylindersCount = 8;
	engineConfiguration->firingOrder = FO_1_5_4_2_6_3_7_8;
}
