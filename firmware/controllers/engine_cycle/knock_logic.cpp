/*
 * @file knock_logic.c
 *
 * @date Apr 04, 2021
 * @author Andrey Gusakov
 */

#include "engine.h"
#include "knock_logic.h"
#include "os_access.h"

int getCylinderKnockBank(uint8_t cylinderIndex) {
	// C/C++ can't index in to bit fields, we have to provide lookup ourselves
	switch (cylinderIndex) {
#if EFI_PROD_CODE
		case 0:
			return CONFIG(knockBankCyl1);
		case 1:
			return CONFIG(knockBankCyl2);
		case 2:
			return CONFIG(knockBankCyl3);
		case 3:
			return CONFIG(knockBankCyl4);
		case 4:
			return CONFIG(knockBankCyl5);
		case 5:
			return CONFIG(knockBankCyl6);
		case 6:
			return CONFIG(knockBankCyl7);
		case 7:
			return CONFIG(knockBankCyl8);
		case 8:
			return CONFIG(knockBankCyl9);
		case 9:
			return CONFIG(knockBankCyl10);
		case 10:
			return CONFIG(knockBankCyl11);
		case 11:
			return CONFIG(knockBankCyl12);
#endif
		default:
			return 0;
	}
}
