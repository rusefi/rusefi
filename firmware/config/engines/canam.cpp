
#include "pch.h"
#include "defaults.h"
#include "proteus_meta.h"
#include "canam.h"

// set engine_type 54
// https://www.youtube.com/watch?v=j8DOFp02QDY
void setMaverickX3() {
    strcpy(engineConfiguration->engineMake, "Rotax");
    strcpy(engineConfiguration->engineCode, "900 ACE");

    engineConfiguration->cylindersCount = 3;
    engineConfiguration->firingOrder = FO_1_3_2;
    engineConfiguration->injectionMode = IM_SEQUENTIAL;

#if HW_PROTEUS
	setProteusEtbIO();
#endif // HW_PROTEUS

}