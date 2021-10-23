/*
 * honda_k_dbc.cpp
 *
 * @date Oct 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "honda_k_dbc.h"

#if HW_PROTEUS & EFI_PROD_CODE
#include "proteus_meta.h"
#endif

/**
 * PROTEUS_HONDA_ELEMENT_2003
 */
void setProteusHondaElement2003(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 2.4;
	engineConfiguration->trigger.type = TT_HONDA_K_12_1;

	strcpy(CONFIG(engineMake), ENGINE_MAKE_HONDA);
	strcpy(CONFIG(engineCode), "K24");
	strcpy(CONFIG(vehicleName), "test");

#if HW_PROTEUS & EFI_PROD_CODE
	engineConfiguration->triggerInputPins[0] = PROTEUS_DIGITAL_2;
	engineConfiguration->camInputs[0] = PROTEUS_DIGITAL_4;
	engineConfiguration->camInputs[1 * CAMS_PER_BANK] = PROTEUS_DIGITAL_1;
#endif
}

void setProteusHondaOBD2A(DECLARE_CONFIG_PARAMETER_SIGNATURE) {

}
