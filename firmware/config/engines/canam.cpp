
#include "pch.h"
#include "defaults.h"
#include "proteus_meta.h"
#include "canam.h"

// set engine_type 54
void setMaverickX3() {
    engineConfiguration->cylindersCount = 3;
    // engineConfiguration->firingOrder = FO_;
    engineConfiguration->injectionMode = IM_SEQUENTIAL;

#if HW_PROTEUS
	setProteusEtbIO();
#endif // HW_PROTEUS

}