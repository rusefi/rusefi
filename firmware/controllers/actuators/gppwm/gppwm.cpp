/**
 * @file gppwm.cpp
 * @brief General Purpose PWM (GPPWM) outputs.
 *
 * Drives a set of user-configurable channels that produce either an on/off or a
 * PWM signal. Each channel's duty/state is looked up from a configurable input
 * (RPM, CLT, TPS, etc.) via 1D/2D tables, allowing arbitrary auxiliary devices
 * (fans, pumps, solenoids) to be controlled without dedicated firmware code.
 */

#include "pch.h"

#include "gppwm_channel.h"
#include "dc_motor.h"
#include "dc_motors.h"

static GppwmChannel channels[GPPWM_CHANNELS];
static OutputPin pins[GPPWM_CHANNELS];
static SimplePwm outputs[GPPWM_CHANNELS];

typedef Map3D<GPPWM_RPM_COUNT, GPPWM_LOAD_COUNT, uint8_t, int16_t, int16_t> gppwm_Map3D_t;

static gppwm_Map3D_t table1{"gppwm1"};
static gppwm_Map3D_t table2{"gppwm2"};
static gppwm_Map3D_t table3{"gppwm3"};
static gppwm_Map3D_t table4{"gppwm4"};

static gppwm_Map3D_t* const tables[] = {
	&table1,
	&table2,
	&table3,
	&table4,
};

static const char *channelNames[GPPWM_CHANNELS] = { "GPPWM#1",
"GPPWM#2",
"GPPWM#3",
"GPPWM#4",
 };


// H-bridge as a general-purpose output, see https://github.com/rusefi/rusefi/issues/9673:
// duty from a GPPWM channel is forwarded into a DC motor slot instead of a plain pin
struct DcMotorPwmWrapper : public IPwm {
	DcMotor* motor = nullptr;

	void setSimplePwmDutyCycle(float dutyCycle) override {
		if (motor) {
			motor->set(dutyCycle);
		}
	}
};

static DcMotorPwmWrapper dcOutputs[GPPWM_CHANNELS];

static_assert(DC_Gppwm4 - DC_Gppwm1 + 1 == GPPWM_CHANNELS);

// Returns an initialized DC motor if some H-bridge is configured to act as this GPPWM channel's output
static DcMotor* getDcMotorForGppwmChannel(size_t channelIndex) {
	auto function = (dc_function_e)(DC_Gppwm1 + channelIndex);

	for (size_t dcIndex = 0; dcIndex < ETB_COUNT; dcIndex++) {
		if (engineConfiguration->etbFunctions[dcIndex] == function) {
			return initDcMotor("GPPWM disable", engineConfiguration->etbIo[dcIndex], dcIndex,
					engineConfiguration->etb_use_two_wires);
		}
	}

	return nullptr;
}

void initGpPwm() {
	for (size_t i = 0; i < efi::size(channels); i++) {
		auto& cfg = engineConfiguration->gppwm[i];

		DcMotor* dcMotor = getDcMotorForGppwmChannel(i);

		// If no pin and no H-bridge, don't enable this channel.
		if (!dcMotor && !isBrainPinValid(cfg.pin)) {
			continue;
		}

		// Determine frequency and whether PWM is enabled
		float freq = cfg.pwmFrequency;
		bool usePwm = freq > 0;

		IPwm* pwm = nullptr;
		OutputPin* outputPin = nullptr;

		if (dcMotor) {
			// An H-bridge only drives while its disable pin is released
			dcMotor->enable();
			dcOutputs[i].motor = dcMotor;
			// Both modes work; in PWM mode the H-bridge switches at etbFreq, not at cfg.pwmFrequency
			pwm = &dcOutputs[i];
		} else {
			// Setup pin & pwm
			pins[i].initPin("gp pwm", cfg.pin);
			outputPin = &pins[i];
			if (usePwm) {
				startSimplePwm(&outputs[i], channelNames[i], &engine->scheduler, &pins[i], freq, 0);
				pwm = &outputs[i];
			}
		}

		// Set up this channel's lookup table
		tables[i]->initTable(cfg.table, cfg.rpmBins, cfg.loadBins);

		// Finally configure the channel
		channels[i].init(usePwm, pwm, outputPin, tables[i], &cfg);
	}
}

void updateGppwm() {
	for (size_t i = 0; i < efi::size(channels); i++) {
		auto result = channels[i].update(i);

		engine->outputChannels.gppwmOutput[i] = result.Result;
		engine->outputChannels.gppwmXAxis[i] = result.X;
		engine->outputChannels.gppwmYAxis[i] = result.Y;
	}
}
