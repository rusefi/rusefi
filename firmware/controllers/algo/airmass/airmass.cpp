#include "airmass.h"

AirmassModelBase::AirmassModelBase(const ValueProvider3D& veTable) : m_veTable(&veTable) {}

float AirmassModelBase::getVe(int rpm, float load) const {
	efiAssert(OBD_PCM_Processor_Fault, m_veTable != nullptr, "VE table null", 0);

	// TODO: allow override of the Y axis value based on a config field
	return m_veTable->getValue(rpm, load);
}
