#pragma once

#include "engine.h"

class InjectorModelBase {
public:
	void prepare();
	floatms_t getInjectionDuration(float fuelMassGram) const;

	virtual floatms_t getDeadtime() const = 0;
	virtual float getInjectorMassFlowRate() const = 0;

private:
	float m_deadtime = 0;
	float m_massFlowRate = 0;
};

class InjectorModel : public InjectorModelBase {
public:
	DECLARE_ENGINE_PTR;

	floatms_t getDeadtime() const override;
	float getInjectorMassFlowRate() const override;
};
