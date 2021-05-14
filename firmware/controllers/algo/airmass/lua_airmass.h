#pragma once

#include "airmass.h"

class LuaAirmass final : public AirmassModelBase {
public:
	AirmassResult getAirmass(int /*rpm*/) const override {
		return m_airmass;
	}

	void setAirmass(AirmassResult airmass) {
		m_airmass = airmass;
	}

private:
	AirmassResult m_airmass;
};
