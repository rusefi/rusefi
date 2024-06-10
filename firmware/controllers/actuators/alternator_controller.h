/**
 * @file    alternator_controller.h
 * @brief   alternator controller
 *
 * @date Apr 6, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 */

#pragma once

void initAlternatorCtrl();

void setAltPFactor(float p);
void setAltIFactor(float p);
void setAltDFactor(float p);

class AlternatorController : public EngineModule, public ClosedLoopController<float, percent_t> {
public:
	void init();
	void pidReset();

	// EngineModule implementation
	void onFastCallback() override;
	void onConfigurationChange(engine_configuration_s const * previousConfiguration) override;

	// ClosedLoopController implementation
	expected<float> getSetpoint() override;
	expected<float> observePlant() override;
	expected<percent_t> getOpenLoop(float setpoint) override;
	expected<percent_t> getClosedLoop(float targetVoltage, float vBattVoltage) override;
	void setOutput(expected<percent_t> outputValue) override;

private:
	Pid alternatorPid;
};

