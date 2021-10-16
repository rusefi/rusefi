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

void setProteusHondaElement2003(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 2.4;
	engineConfiguration->trigger.type = TT_HONDA_K_12_1;
}

void setProteusHondaOBD2A(DECLARE_CONFIG_PARAMETER_SIGNATURE) {

}
