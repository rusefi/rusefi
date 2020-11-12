#pragma once

#include "engine.h"
#include "expected.h"

struct IInjectorModel {
	virtual void prepare() = 0;
	virtual floatms_t getInjectionDuration(float fuelMassGram) const = 0;
};

class InjectorModelBase : public IInjectorModel {
public:
	void prepare() override;
	floatms_t getInjectionDuration(float fuelMassGram) const override;

	virtual floatms_t getDeadtime() const = 0;
	virtual float getInjectorMassFlowRate() const = 0;
	virtual float getInjectorFlowRatio() const = 0;
	virtual expected<float> getAbsoluteRailPressure() const = 0;

	virtual void postState(float deadTime) const { (void)deadTime; };

private:
	float m_deadtime = 0;
	float m_massFlowRate = 0;
};

class InjectorModel : public InjectorModelBase {
public:
	DECLARE_ENGINE_PTR;

	void postState(float deadtime) const override;
	floatms_t getDeadtime() const override;
	float getInjectorMassFlowRate() const override;
	float getInjectorFlowRatio() const override;
	expected<float> getAbsoluteRailPressure() const override;
};
