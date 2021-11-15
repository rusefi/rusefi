
#include "pch.h"

#include "gppwm_channel.h"

static GppwmChannel channels[GPPWM_CHANNELS];
static OutputPin pins[GPPWM_CHANNELS];
static SimplePwm outputs[GPPWM_CHANNELS];

static gppwm_Map3D_t table1;
static gppwm_Map3D_t table2;
static gppwm_Map3D_t table3;
static gppwm_Map3D_t table4;

static gppwm_Map3D_t* tables[] = {
	&table1,
	&table2,
	&table3,
	&table4,
};

void initGpPwm(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	for (size_t i = 0; i < efi::size(channels); i++) {
		auto& cfg = CONFIG(gppwm)[i];

		// If no pin, don't enable this channel.
		if (!isBrainPinValid(cfg.pin)) {
			continue;
		}

		// Determine frequency and whether PWM is enabled
		float freq = cfg.pwmFrequency;
		bool usePwm = freq > 0;

		// Setup pin & pwm
		pins[i].initPin("gp pwm", cfg.pin);
		if (usePwm) {
			startSimplePwm(&outputs[i], "gp pwm", &engine->executor, &pins[i], freq, 0);
		}

		// Set up this channel's lookup table
		tables[i]->init(cfg.table, cfg.loadBins, cfg.rpmBins);

		// Finally configure the channel
		channels[i].inject(PASS_ENGINE_PARAMETER_SIGNATURE);
		channels[i].init(usePwm, &outputs[i], &pins[i], tables[i], &cfg);
	}
}

void updateGppwm() {
	// There are only 8 debug float fields, this will overflow if more channels
	static_assert(efi::size(channels) <= 8);

	for (size_t i = 0; i < efi::size(channels); i++) {
		float result = channels[i].update();

#ifdef EFI_TUNER_STUDIO
		if (CONFIG(debugMode) == DBG_GPPWM) {
			scaled_channel<float>* debugFloats = &tsOutputChannels.debugFloatField1;
			debugFloats[i] = result;
		}
#endif
	}
}
