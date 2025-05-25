
#include "pch.h"
#include "defaults.h"

#ifdef HW_HELLEN
#include "hellen_meta.h"
#endif // HW_HELLEN

void setFordEcoboost() {
  engineConfiguration->hpfpCamLobes = 4;
  setHpfpLobeProfileAngle(engineConfiguration->hpfpCamLobes);
  engineConfiguration->vvtMode[1] = VVT_BOSCH_QUICK_START;
	setInline4();

#ifdef HW_HELLEN_4K_GDI
#endif // HW_HELLEN_4K_GDI
}
