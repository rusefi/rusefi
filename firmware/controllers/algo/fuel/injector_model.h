#pragma once

#include <rusefi/expected.h>
#include "injector_model_generated.h"
#include "engine_module.h"

struct IInjectorModel : public EngineModule {
	virtual void prepare() = 0;
	virtual floatms_t getInjectionDuration(float fuelMassGram) const = 0;
	virtual float getFuelMassForDuration(floatms_t duration) const = 0;
	virtual floatms_t getDeadtime() const = 0;
};

class InjectorModelBase : public IInjectorModel, public injector_model_s {
public:
	void prepare() override;
	floatms_t getInjectionDuration(float fuelMassGram) const override;
	float getFuelMassForDuration(floatms_t duration) const override;

	virtual float getInjectorFlowRatio() = 0;
	virtual void updateState() = 0;
	virtual expected<float> getFuelDifferentialPressure() const = 0;
	virtual expected<float> getFuelPressure() const = 0;

	virtual float getBaseFlowRate() const = 0;

	virtual InjectorNonlinearMode getNonlinearMode() const = 0;
	floatms_t getBaseDurationImpl(float fuelMassGram) const;
	virtual floatms_t correctInjectionPolynomial(float floatms_t) const;

	// Ford small pulse model
	virtual float getSmallPulseFlowRate() const = 0;
	virtual float getSmallPulseBreakPoint() const = 0;

private:
	// Mass flow rate for large-pulse flow, g/s
	float m_massFlowRate = 0;

	// Break point below which the "small pulse" slope is used, grams
	float m_smallPulseBreakPoint = 0;

	// Flow rate for small pulses, g/s
	float m_smallPulseFlowRate = 0;

	// Correction adder for small pulses to correct for small/large pulse kink, ms
	float m_smallPulseOffset = 0;
};

class InjectorModelWithConfig : public InjectorModelBase {
public:
	InjectorModelWithConfig(const injector_s* const cfg);

	floatms_t getDeadtime() const override;
	float getBaseFlowRate() const override;
	float getInjectorFlowRatio() override;
	void updateState() override;
	expected<float> getFuelDifferentialPressure() const override;
	expected<float> getFuelPressure() const override;
	floatms_t getInjectionDuration(float fuelMassGram) const override;

	using interface_t = IInjectorModel; // Mock interface
private:
	[[nodiscard]] virtual injector_compensation_mode_e getInjectorCompensationMode() const = 0;
	[[nodiscard]] virtual float getFuelReferencePressure() const = 0;

	const injector_s* const m_cfg;
};

struct InjectorModelPrimary : InjectorModelWithConfig {
	InjectorModelPrimary();

	InjectorNonlinearMode getNonlinearMode() const override;

	// Ford small pulse model
	float getSmallPulseFlowRate() const override;
	float getSmallPulseBreakPoint() const override;
private:
	[[nodiscard]] injector_compensation_mode_e getInjectorCompensationMode() const final;
	[[nodiscard]] float getFuelReferencePressure() const final;
};

struct InjectorModelSecondary : InjectorModelWithConfig {
	InjectorModelSecondary();

	InjectorNonlinearMode getNonlinearMode() const override;

	// Ford small pulse model
	float getSmallPulseFlowRate() const override;
	float getSmallPulseBreakPoint() const override;
private:
	[[nodiscard]] injector_compensation_mode_e getInjectorCompensationMode() const final;
	[[nodiscard]] float getFuelReferencePressure() const final;
};
