/*
 * boost_control.h
 *
 *  Created on: 18. aug. 2019
 *      Author: Ola Ruud
 */
#pragma once

#include "closed_loop_controller.h"
#include "efi_pid.h"
#include "boost_control_generated.h"

#include "Map2D.h"

struct IPwm;

class BoostController : public EngineModule, public boost_control_s, public ClosedLoopController<float, percent_t>  {
public:
	void init(
        IPwm* const pmw,
        const ValueProvider3D* const openLoopMap,
        const ValueProvider3D* const closedLoopTargetMap,
        const ValueProvider2D& cltMultiplierProvider,
        const ValueProvider2D& iatMultiplierProvider,
        const ValueProvider2D& cltAdderProvider,
        const ValueProvider2D& iatAdderProvider,
        pid_s* const pidParams
    );

	void onFastCallback() override;
	void resetLua();

	// Called when the configuration may have changed.  Controller will
	// reset if necessary.
	void onConfigurationChange(engine_configuration_s const * previousConfig) override;

	// Helpers for individual parts of boost control
	expected<float> observePlant() override;
	expected<float> getSetpoint() override;

	expected<percent_t> getOpenLoop(float target) override;
	expected<percent_t> getClosedLoop(float target, float manifoldPressure) override;

	void setOutput(expected<percent_t> outputValue) override;

private:
	percent_t getClosedLoopImpl(float target, float manifoldPressure);

    float getBoostControlDutyCycleWithTemperatureCorrections(const float rpm, const float driverIntent) const;
    std::optional<float> getBoostControlTargetTemperatureAdder() const;
    std::optional<float> getBoostTemperatureCorrection(
        const SensorType sensorType,
        const ValueProvider2D& correctionCurve
    ) const;

	Pid m_pid;

	const ValueProvider3D* m_openLoopMap = nullptr;
	const ValueProvider3D* m_closedLoopTargetMap = nullptr;
    const ValueProvider2D* m_cltBoostCorrMap = nullptr;
    const ValueProvider2D* m_iatBoostCorrMap = nullptr;
    const ValueProvider2D* m_cltBoostAdderMap = nullptr;
    const ValueProvider2D* m_iatBoostAdderMap = nullptr;
	IPwm* m_pwm = nullptr;
};

void startBoostPin();
void initBoostCtrl();
void setDefaultBoostParameters();
